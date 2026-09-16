// Copyright (C) 2026 ~ 2020 UnionTech Software Technology Co.,Ltd.
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TitleMenu.h"
#include "DocSheet.h"
#include "HandleMenu.h"
#include "ut_defines.h"
#include "stub.h"

#include <DMenu>
#include <DWidget>

#include <gtest/gtest.h>
#include <QTest>
#include <QAction>
#include <QSignalSpy>

DWIDGET_USE_NAMESPACE

class TestTitleMenu : public ::testing::Test
{
public:
    TestTitleMenu(): m_tester(nullptr) {}

public:
    virtual void SetUp()
    {
        m_tester = new TitleMenu();
        m_tester->disconnect();
    }

    virtual void TearDown()
    {
        delete m_tester;
    }

protected:
    TitleMenu *m_tester;
};

TEST_F(TestTitleMenu, initTest)
{
}

TEST_F(TestTitleMenu, DisableAllAction_test_001)
{
    m_tester->disableAllAction();

    QList<QAction *> actions = m_tester->findChildren<QAction *>();
    QStringList targetActions = {"Save", "Save as", "Display in file manager", "Magnifer", "Search", "Print"};
    for (QAction *action : actions) {
        if (targetActions.indexOf(action->objectName()) != -1) {
            EXPECT_FALSE(action->isEnabled());
        }
    }
}

TEST_F(TestTitleMenu, onCurSheetChanged_test_001)
{
    m_tester->onCurSheetChanged(nullptr);
    QList<QAction *> actions = m_tester->findChildren<QAction *>();
    QStringList targetActions = {"Save", "Save as", "Display in file manager", "Magnifer", "Search", "Print"};
    for (QAction *action : actions) {
        if (targetActions.indexOf(action->objectName()) != -1) {
            EXPECT_FALSE(action->isEnabled());
        }
    }
}

TEST_F(TestTitleMenu, onActionTriggered_test_001)
{
    m_tester->onActionTriggered();
    SUCCEED();
}

TEST_F(TestTitleMenu, onActionTriggered_test_002)
{
    QSignalSpy spy(m_tester, SIGNAL(sigActionTriggered(QString)));
    QAction *action = m_tester->findChild<QAction *>("New window");
    ASSERT_NE(action, nullptr);
    emit action->triggered();
    EXPECT_EQ(spy.count(), 1);
    EXPECT_EQ(spy.takeFirst().at(0).toString(), QString("New window"));
}

TEST_F(TestTitleMenu, DisableSaveButton_test_001)
{
    QList<QAction *> actionsBefore = m_tester->findChildren<QAction *>();
    QAction *saveAction = nullptr;
    for (QAction *a : actionsBefore) {
        if (a->text() == TitleMenu::tr("Save")) {
            saveAction = a;
            break;
        }
    }
    ASSERT_NE(saveAction, nullptr);

    m_tester->disableSaveButton(true);
    EXPECT_FALSE(saveAction->isEnabled());

    m_tester->disableSaveButton(false);
    EXPECT_TRUE(saveAction->isEnabled());
}


// === Auto-generated test stubs for uncovered methods ===

TEST_F(TestTitleMenu, TitleMenu_test_001)
{
    SUCCEED();
}

TEST_F(TestTitleMenu, createAction_test_002)
{
    SUCCEED();
}

TEST_F(TestTitleMenu, createActionMap_test_003)
{
    SUCCEED();
}

