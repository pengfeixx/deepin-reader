#include "DocShowShellWidget.h"
#include <QVBoxLayout>
#include "FileViewWidget.h"
#include "controller/DataManager.h"

DocShowShellWidget::DocShowShellWidget(CustomWidget *parent)
    : CustomWidget ("DocShowShellWidget", parent)
{
    initWidget();
    initConnections();
}

DocShowShellWidget::~DocShowShellWidget()
{
    if (m_pFileAttrWidget) {
        m_pFileAttrWidget->deleteLater();
        m_pFileAttrWidget = nullptr;
    }
}

void DocShowShellWidget::resizeEvent(QResizeEvent *event)
{
    if (m_pFindWidget != nullptr) {
        int nParentWidth = this->width();
        int nWidget = m_pFindWidget->width();
        m_pFindWidget->move(nParentWidth - nWidget - 20, 20);
    }

    CustomWidget::resizeEvent(event);
}

//  获取文件的基本数据，　进行展示
void DocShowShellWidget::slotShowFileAttr()
{
    if (m_pFileAttrWidget == nullptr) {
        m_pFileAttrWidget = new FileAttrWidget;
    }
    m_pFileAttrWidget->showScreenCenter();
}

//  搜索框
void DocShowShellWidget::slotShowFindWidget()
{
    if (m_pFindWidget == nullptr) {
        m_pFindWidget = new FindWidget(this);
    }

    int nParentWidth = this->width();
    int nWidget = m_pFindWidget->width();
    m_pFindWidget->move(nParentWidth - nWidget - 20, 20);

    m_pFindWidget->show();
    m_pFindWidget->raise();
}

//  注释窗口
void DocShowShellWidget::slotOpenNoteWidget(const QString &msgContent)
{
    if (m_pFileViewNoteWidget == nullptr) {
        m_pFileViewNoteWidget = new FileViewNoteWidget(this);
    }
    m_pFileViewNoteWidget->setEditText("");
    m_pFileViewNoteWidget->setPointAndPage(msgContent);
    m_pFileViewNoteWidget->setNoteUuid("");
    m_pFileViewNoteWidget->setNotePage("");

    QRect rrect = this->rect();
    QPoint point = this->mapToGlobal(rrect.bottomRight());
    int nRight = point.x();
    DataManager::instance()->setSmallNoteWidgetSize(m_pFileViewNoteWidget->size());
    m_pFileViewNoteWidget->showWidget(nRight);
}

//  显示 当前 注释
void DocShowShellWidget::slotShowNoteWidget(const QString &contant)
{
    QStringList t_strList = contant.split(QString("%"), QString::SkipEmptyParts);
    if (t_strList.count() == 3) {
        QString t_strUUid = t_strList.at(0);
        QString t_page = t_strList.at(2);

        if (m_pFileViewNoteWidget == nullptr) {
            m_pFileViewNoteWidget = new FileViewNoteWidget(this);
        }

        auto pDocummentProxy = DocummentProxy::instance();
        QString contant;
        pDocummentProxy->getAnnotationText(t_strUUid, contant, t_page.toInt());

        m_pFileViewNoteWidget->setNoteUuid(t_strUUid);
        m_pFileViewNoteWidget->setNotePage(t_page);
        m_pFileViewNoteWidget->setEditText(contant);
        m_pFileViewNoteWidget->setPointAndPage("");
        DataManager::instance()->setSmallNoteWidgetSize(m_pFileViewNoteWidget->size());

        bool t_bHigh = false; // 点击位置是否是高亮
//        QRect rrect = this->rect();
        QPoint point;// = this->mapToGlobal(rrect.bottomRight());// 鼠标点击位置
//        int nRight = point.x();
//        m_pFileViewNoteWidget->showWidget(nRight);

        DataManager::instance()->mousePressLocal(t_bHigh, point);
        if(t_bHigh){
            m_pFileViewNoteWidget->showWidget(point);
        }
    }
}

void DocShowShellWidget::initConnections()
{
    connect(this, SIGNAL(sigShowFileAttr()), this, SLOT(slotShowFileAttr()));
    connect(this, SIGNAL(sigShowFileFind()), this, SLOT(slotShowFindWidget()));

    connect(this, SIGNAL(sigOpenNoteWidget(const QString &)), this, SLOT(slotOpenNoteWidget(const QString &)));
    connect(this, SIGNAL(sigShowNoteWidget(const QString &)), this, SLOT(slotShowNoteWidget(const QString &)));
}

int DocShowShellWidget::dealWithData(const int &msgType, const QString &msgContent)
{
    switch (msgType) {
    case MSG_OPERATION_ATTR:        //  打开该文件的属性信息
        emit sigShowFileAttr();
        return ConstantMsg::g_effective_res;
    case MSG_OPERATION_FIND:        //  搜索
        emit sigShowFileFind();
        return ConstantMsg::g_effective_res;
    case MSG_OPERATION_TEXT_ADD_ANNOTATION:     //  添加注释
        emit sigOpenNoteWidget(msgContent);
        return ConstantMsg::g_effective_res;
    case MSG_OPERATION_TEXT_SHOW_NOTEWIDGET:    //  显示注释窗口
        emit sigShowNoteWidget(msgContent);
        return ConstantMsg::g_effective_res;
    case MSG_NOTIFY_KEY_MSG : {    //  最后一个处理通知消息
        if ("Ctrl+F" == msgContent) {
            emit sigShowFileFind();
            return ConstantMsg::g_effective_res;
        }
        break;
    }
    }

    return 0;
}

void DocShowShellWidget::initWidget()
{
    auto layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(new FileViewWidget);

    this->setLayout(layout);
}
