#ifndef MAINSHOWSPLITTER_H
#define MAINSHOWSPLITTER_H

#include "FileViewWidget.h"
#include "LeftSidebarWidget.h"
#include "header/IMsgObserver.h"
#include "header/MsgSubject.h"
#include <DSplitter>
#include <QResizeEvent>

DWIDGET_USE_NAMESPACE

/**
 * @brief The MainShowDataWidget class
 * @brief   主要用于显示文档 和 操作界面, 分割器
 */

class MainShowSplitter : public DSplitter, public IMsgObserver
{
    Q_OBJECT
public:
    MainShowSplitter(DWidget *parent = nullptr);
    ~MainShowSplitter() override;

private:
    void initWidgets();

private:
    void setSidebarVisible(const bool &) const;

private:
    LeftSidebarWidget *m_pLeftShowWidget = nullptr; //  侧边栏
    FileViewWidget *m_pFileViewWidget = nullptr; //  显示文档区域

    MsgSubject *m_pMsgSubject = nullptr;

    // IObserver interface
public:
    int update(const int &, const QString &) override;
};

#endif // MAINSHOWDATAWIDGET_H
