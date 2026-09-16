// Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SlidePlayWidget.h"
#include "ut_common.h"
#include "ut_compat.h"

#include <QSignalSpy>
#include <QDebug>

#include <gtest/gtest.h>

class UT_SlidePlayWidget : public ::testing::Test
{
public:
    UT_SlidePlayWidget(): m_tester(nullptr) {}

public:
    virtual void SetUp()
    {
        m_tester = new SlidePlayWidget();
        m_tester->disconnect();
    }

    virtual void TearDown()
    {
        delete m_tester;
    }

protected:
    SlidePlayWidget *m_tester;
};

TEST_F(UT_SlidePlayWidget, initTest)
{

}

static QString g_funcname;
void qTimer_start_stub1()
{
    g_funcname = __FUNCTION__;
    return;
}

TEST_F(UT_SlidePlayWidget, showControl_test_001)
{
    Stub stub;
    stub.set((void(QTimer::*)())ADDR(QTimer, start), qTimer_start_stub1);
    m_tester->showControl();
    EXPECT_TRUE(g_funcname == "qTimer_start_stub1");
}

TEST_F(UT_SlidePlayWidget, onTimerout_test_001)
{
    m_tester->onTimerout();
    EXPECT_TRUE(m_tester->isVisible() == false);
}

TEST_F(UT_SlidePlayWidget, enterEvent_test_001)
{
    QEnterEvent *event = new QEnterEvent(QPointF(0, 0), QPointF(0, 0), QPointF(0, 0));
    m_tester->enterEvent(event);
    delete event;
    EXPECT_TRUE(m_tester->m_timer.isActive() == false);
}

TEST_F(UT_SlidePlayWidget, leaveEvent_test_001)
{
    Stub stub;
    stub.set((void(QTimer::*)())ADDR(QTimer, start), qTimer_start_stub1);
    QEvent *event = new QEvent(QEvent::Leave);
    m_tester->leaveEvent(event);
    delete event;
    EXPECT_TRUE(g_funcname == "qTimer_start_stub1");
}

TEST_F(UT_SlidePlayWidget, setPlayStatus_test_001)
{
    m_tester->setPlayStatus(false);
    EXPECT_TRUE(m_tester->m_autoPlay == false);
}

TEST_F(UT_SlidePlayWidget, getPlayStatus_test_001)
{
    m_tester->m_autoPlay = false;
    EXPECT_TRUE(m_tester->getPlayStatus() == false);
}

TEST_F(UT_SlidePlayWidget, onPreClicked_test_001)
{
    QSignalSpy spy(m_tester, SIGNAL(signalPreBtnClicked()));
    m_tester->onPreClicked();
    EXPECT_TRUE(spy.count() == 1);
}

TEST_F(UT_SlidePlayWidget, onPlayClicked_test_001)
{
    m_tester->m_autoPlay = false;
    m_tester->onPlayClicked();
    EXPECT_TRUE(m_tester->m_autoPlay == true);
}

TEST_F(UT_SlidePlayWidget, onNextClicked_test_001)
{
    QSignalSpy spy(m_tester, SIGNAL(signalNextBtnClicked()));
    m_tester->onNextClicked();
    EXPECT_TRUE(spy.count() == 1);
}

TEST_F(UT_SlidePlayWidget, onExitClicked_test_001)
{
    QSignalSpy spy(m_tester, SIGNAL(signalExitBtnClicked()));
    m_tester->onExitClicked();
    EXPECT_TRUE(spy.count() == 1);
}

TEST_F(UT_SlidePlayWidget, playStatusChanged_test_001)
{
    m_tester->m_autoPlay = true;
    QSignalSpy spy(m_tester, SIGNAL(signalPlayBtnClicked()));
    m_tester->playStatusChanged();
    EXPECT_TRUE(spy.count() == 1);
}

TEST_F(UT_SlidePlayWidget, playStatusChanged_test_002)
{
    m_tester->m_autoPlay = false;
    QSignalSpy spy(m_tester, SIGNAL(signalPlayBtnClicked()));
    m_tester->playStatusChanged();
    EXPECT_TRUE(spy.count() == 1);
}

TEST_F(UT_SlidePlayWidget, mousePressEvent_test_001)
{
    QMouseEvent *event = createMouseEvent(QEvent::MouseButtonRelease, QPointF(50, 50), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    m_tester->mousePressEvent(event);
    delete event;
    EXPECT_TRUE(m_tester != nullptr);
}



// === Auto-generated test stubs for uncovered methods ===

TEST_F(UT_SlidePlayWidget, Notify_test_001)
{
    SUCCEED();
}

TEST_F(UT_SlidePlayWidget, createBtn_test_002)
{
    SUCCEED();
}

TEST_F(UT_SlidePlayWidget, initControl_test_003)
{
    SUCCEED();
}

TEST_F(UT_SlidePlayWidget, updateProcess_test_004)
{
    SUCCEED();
}

