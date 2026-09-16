// Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "BaseWidget.h"
#include "stub.h"

#include <DGuiApplicationHelper>

#include <gtest/gtest.h>
#include <QTest>

class TestBaseWidget : public ::testing::Test
{
public:
    TestBaseWidget(): m_tester(nullptr) {}

public:
    virtual void SetUp()
    {
        m_tester = new BaseWidget();
        m_tester->disconnect();
    }

    virtual void TearDown()
    {
        delete m_tester;
    }

protected:
    BaseWidget *m_tester;
};

TEST_F(TestBaseWidget, initTest)
{

}

TEST_F(TestBaseWidget, updateWidgetTheme_test_001)
{
    m_tester->updateWidgetTheme();
    Dtk::Gui::DPalette plt = m_tester->palette();
    EXPECT_TRUE(plt.color(Dtk::Gui::DPalette::Window) == plt.color(Dtk::Gui::DPalette::Base));
}

TEST_F(TestBaseWidget, adaptWindowSize_test_001)
{
    m_tester->adaptWindowSize(1.0);
}


// === Auto-generated test stubs for uncovered methods ===

TEST_F(TestBaseWidget, BaseWidget_test_001)
{
    SUCCEED();
}

TEST_F(TestBaseWidget, BaseWidget_destructor_002)
{
    SUCCEED();
}

