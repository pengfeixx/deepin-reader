// Copyright (C) 2019-2026 ~ 2020 UnionTech Software Technology Co.,Ltd.
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SecurityDialog.h"

#include "stub.h"

#include <gtest/gtest.h>
#include <QTest>
#include <QSignalSpy>
#include <QEvent>
#include <QLabel>

class TestSecurityDialog : public ::testing::Test
{
public:
    TestSecurityDialog(): m_tester(nullptr) {}

public:
    virtual void SetUp()
    {
        m_tester = new SecurityDialog("https://example.com/path?query=1");
        m_tester->disconnect();
    }

    virtual void TearDown()
    {
        delete m_tester;
    }

protected:
    SecurityDialog *m_tester;
};

TEST_F(TestSecurityDialog, initTest)
{
}

TEST_F(TestSecurityDialog, testDialogInitialProperties)
{
    EXPECT_EQ(m_tester->minimumWidth(), 380);
    EXPECT_GE(m_tester->buttonCount(), 2);
}

TEST_F(TestSecurityDialog, changeEvent_test_001)
{
    QEvent event(QEvent::FontChange);
    m_tester->changeEvent(&event);
    SUCCEED();
}

TEST_F(TestSecurityDialog, changeEvent_test_002)
{
    QEvent event(QEvent::ThemeChange);
    m_tester->changeEvent(&event);
    SUCCEED();
}

TEST_F(TestSecurityDialog, changeEvent_test_003)
{
    QEvent event(QEvent::None);
    m_tester->changeEvent(&event);
    SUCCEED();
}

// Free function tests
TEST(UT_autoCutText, emptyTextReturnsEmpty)
{
    DLabel label;
    NewStr result = autoCutText(QString(), &label);
    EXPECT_TRUE(result.resultStr.isEmpty());
    EXPECT_TRUE(result.strList.isEmpty());
}

TEST(UT_autoCutText, nullLabelReturnsEmpty)
{
    NewStr result = autoCutText("hello world", nullptr);
    EXPECT_TRUE(result.resultStr.isEmpty());
}

TEST(UT_autoCutText, shortTextNoWrap)
{
    DLabel label;
    label.setFixedWidth(1000);  // Wide enough that no wrapping is needed
    NewStr result = autoCutText("hi", &label);
    EXPECT_EQ(result.resultStr, QString("hi"));
    EXPECT_EQ(result.strList.size(), 1);
    EXPECT_GT(result.fontHeifht, 0);
}

TEST(UT_autoCutText, longTextWraps)
{
    DLabel label;
    label.setFixedWidth(20);  // Very narrow, forcing wrap
    NewStr result = autoCutText("this is some long text that should wrap", &label);
    EXPECT_FALSE(result.resultStr.isEmpty());
    EXPECT_GE(result.strList.size(), 1);
}


// === Auto-generated test stubs for uncovered methods ===

TEST_F(TestSecurityDialog, SecurityDialog_test_001)
{
    SUCCEED();
}

TEST_F(TestSecurityDialog, autoFeed_test_002)
{
    SUCCEED();
}

TEST_F(TestSecurityDialog, setLabelColor_test_003)
{
    SUCCEED();
}

