#include "FileAttrWidget.h"
#include <DWidgetUtil>

FileAttrWidget::FileAttrWidget(DWidget *parent)
    : DWidget(parent)
{
    setFixedSize(QSize(400, 500));
    setAttribute(Qt::WA_ShowModal, true); //  模态对话框， 属性设置

    m_pGridLayout = new QGridLayout;
    m_pGridLayout->setSpacing(0);
    m_pGridLayout->setContentsMargins(40, 20, 20, 40);
    this->setLayout(m_pGridLayout);

    //  桌面 中心显示
    Dtk::Widget::moveToCenter(this);

    initLabels();

    m_pMsgSubject = MsgSubject::getInstance();
    if (m_pMsgSubject) {
        m_pMsgSubject->addObserver(this);
    }
}

FileAttrWidget::~FileAttrWidget()
{
    if (m_pMsgSubject) {
        m_pMsgSubject->removeObserver(this);
    }
}

//  各个 对应的 label 赋值
void FileAttrWidget::setFileAttr() {}

//  初始化 所有的label 显示
void FileAttrWidget::initLabels()
{
    createLabel(0, tr("Title:"));
    createLabel(1, tr("Location:"));
    createLabel(2, tr("Theme:"));
    createLabel(3, tr("Author:"));
    createLabel(4, tr("Keywords:"));
    createLabel(5, tr("Producers:"));
    createLabel(6, tr("Creator:"));
    createLabel(7, tr("Create Time:"));
    createLabel(8, tr("Update Time:"));
    createLabel(9, tr("Format:"));
    createLabel(10, tr("Page's Number:"));
    createLabel(11, tr("Optimize:"));
    createLabel(12, tr("Security:"));
    createLabel(13, tr("Paper Size:"));
    createLabel(14, tr("File Size:"));
}

void FileAttrWidget::createLabel(const int &index, const QString &objName)
{
    DLabel *label = new DLabel(objName);
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label->setFixedWidth(100);
    m_pGridLayout->addWidget(label, index, 0);

    DLabel *labelText = new DLabel();
    labelText->setObjectName(objName);
    labelText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pGridLayout->addWidget(labelText, index, 1);

    m_labelList.append(labelText);
}

int FileAttrWidget::update(const int &, const QString &)
{
    return 0;
}
