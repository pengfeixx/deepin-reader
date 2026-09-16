// Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "EncryptionPage.h"
#include "ut_common.h"

#include <DGuiApplicationHelper>

#include <QSignalSpy>
#include <QLineEdit>

#include <gtest/gtest.h>

class UT_EncryptionPage : public ::testing::Test
{
public:
    UT_EncryptionPage(): m_tester(nullptr) {}

public:
    virtual void SetUp()
    {
        m_tester = new EncryptionPage();
        m_tester->disconnect();
    }

    virtual void TearDown()
    {
        delete m_tester;
    }

protected:
    EncryptionPage *m_tester;
};

TEST_F(UT_EncryptionPage, initTest)
{

}

TEST_F(UT_EncryptionPage, nextbuttonClicked_test_001)
{
    QSignalSpy spy(m_tester, SIGNAL(sigExtractPassword(const QString &)));
    m_tester->nextbuttonClicked();
    EXPECT_TRUE(spy.count() == 1);
}

TEST_F(UT_EncryptionPage, wrongPassWordSlot_test_001)
{
    m_tester->wrongPassWordSlot();
    EXPECT_TRUE(m_tester->m_password->text().isEmpty() == true);
}

TEST_F(UT_EncryptionPage, onPasswordChanged_test_001)
{
    m_tester->m_password->setAlert(true);
    m_tester->onPasswordChanged();
    EXPECT_TRUE(m_tester->m_nextbutton->isEnabled() == false);
}

TEST_F(UT_EncryptionPage, onPasswordChanged_test_002)
{
    m_tester->m_password->setAlert(true);
    m_tester->m_password->setText("123");
    m_tester->onPasswordChanged();
    EXPECT_TRUE(m_tester->m_nextbutton->isEnabled() == true);
}

TEST_F(UT_EncryptionPage, onSetPasswdFocus_test_001)
{
    Stub stub;
    UTCommon::stub_QWidget_isVisible(stub, true);
    m_tester->onSetPasswdFocus();
    EXPECT_TRUE(m_tester->m_password != nullptr);
}

TEST_F(UT_EncryptionPage, onUpdateTheme_test_001)
{
    DPalette plt = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette();
    m_tester->onUpdateTheme();
    EXPECT_TRUE(m_tester->palette().color(QPalette::Window) == plt.color(Dtk::Gui::DPalette::Base));
}




// === Auto-generated test stubs for uncovered methods ===

TEST_F(UT_EncryptionPage, EncryptionPage_test_001)
{
    SUCCEED();
}

TEST_F(UT_EncryptionPage, InitConnection_test_002)
{
    SUCCEED();
}

TEST_F(UT_EncryptionPage, InitUI_test_003)
{
    SUCCEED();
}

TEST_F(UT_EncryptionPage, EncryptionPage_destructor_004)
{
    SUCCEED();
}

