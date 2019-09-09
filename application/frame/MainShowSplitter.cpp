#include "MainShowSplitter.h"
#include "subjectObserver/MsgHeader.h"
#include <QDebug>

MainShowSplitter::MainShowSplitter(DWidget *parent) :
    DSplitter (parent)
{
    setObserverName("MainShowSplitter");

    setChildrenCollapsible(false);      //  子部件 不可以被拉伸到 宽度 为0
    initWidget();

    m_pMsgSubject = MsgSubject::getInstance();
    if (m_pMsgSubject) {
        m_pMsgSubject->addObserver(this);
    }
}

MainShowSplitter::~MainShowSplitter()
{
    if (m_pMsgSubject) {
        m_pMsgSubject->removeObserver(this);
    }
}

void MainShowSplitter::initWidget()
{
    m_pLeftShowWidget = new LeftSidebarWidget();
    insertWidget(0, m_pLeftShowWidget);

    m_pFileViewWidget = new FileViewWidget();
    insertWidget(1, m_pFileViewWidget);

    //  布局 占比
    setStretchFactor(0, 2);
    setStretchFactor(1, 8);
}

//  侧边栏的显隐
void MainShowSplitter::setSidebarVisible(const bool &bVis) const
{
    m_pLeftShowWidget->setVisible(bVis);
}

int MainShowSplitter::dealWithData(const int &msgType, const QString &msgContent)
{
    if (msgType == MSG_SLIDER_SHOW_STATE) {
        setSidebarVisible(msgContent.toInt());
        return  ConstantMsg::g_effective_res;
    }
    return 0;
}

void MainShowSplitter::sendMsg(const int &msgType, const QString &msgContent)
{
    Q_UNUSED(msgType);
    Q_UNUSED(msgContent);
}

void MainShowSplitter::setObserverName(const QString &name)
{
    m_strObserverName = name;
}
