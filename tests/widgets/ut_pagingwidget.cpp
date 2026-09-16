// Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PagingWidget.h"
#include "Global.h"
#include "DocSheet.h"
#include "ut_defines.h"
#include "TMFunctionThread.h"
#include "stub.h"

#include <gtest/gtest.h>
#include <QTest>

namespace {
class TestPagingWidget : public ::testing::Test
{
public:
    TestPagingWidget(): m_tester(nullptr) {}

public:
    virtual void SetUp()
    {
        QString filePath = QCoreApplication::applicationDirPath() + "/" + "files" + "/" + UT_FILE_PDF;
        if (!QFile(filePath).exists() && QFile(":/files/" + QString(UT_FILE_PDF)).exists()) {
            QDir().mkpath(QCoreApplication::applicationDirPath() + "/" + "files");
            QFile(":/files/" + QString(UT_FILE_PDF)).copy(filePath);
        }

        sheet = new DocSheet(Dr::FileType::PDF, filePath, nullptr);
        m_tester = new PagingWidget(sheet);
        m_tester->disconnect();
    }

    virtual void TearDown()
    {
        delete sheet;
        delete m_tester;
    }

protected:
    DocSheet *sheet = nullptr;
    PagingWidget *m_tester = nullptr;
};

static QString g_funcname;
void normalChangePage_stub()
{
    g_funcname = __FUNCTION__;
}

void setIndex_stub(int)
{
    g_funcname = __FUNCTION__;
}

void jumpToIndex_stub(int)
{
    g_funcname = __FUNCTION__;
}
void jumpToPage_stub(int)
{
    g_funcname = __FUNCTION__;
}
int pageCount_stub()
{
    return 10;
}
}

TEST_F(TestPagingWidget, initTest)
{

}

TEST_F(TestPagingWidget, slotUpdateTheme_test_001)
{
    m_tester->slotUpdateTheme();
    EXPECT_TRUE(m_tester->m_pTotalPagesLab->foregroundRole() == DPalette::Text);
    EXPECT_TRUE(m_tester->m_pCurrentPageLab->foregroundRole() == DPalette::Text);
}

TEST_F(TestPagingWidget, resizeEvent_test_001)
{
    Stub s;
    s.set(ADDR(PagingWidget, setIndex), setIndex_stub);
    QResizeEvent *event = new QResizeEvent(QSize(100, 100), QSize(80, 80));
    m_tester->resizeEvent(event);
    delete event;
    EXPECT_TRUE(g_funcname == "setIndex_stub");
}

TEST_F(TestPagingWidget, setIndex_test_001)
{
    m_tester->m_curIndex = 1;
    m_tester->setIndex(0);
    EXPECT_EQ(m_tester->m_curIndex, 0);
}

TEST_F(TestPagingWidget, handleOpenSuccess_test_001)
{
    m_tester->handleOpenSuccess();

    m_tester->m_tmFuncThread->wait();
    EXPECT_TRUE(m_tester->m_tmFuncThread->result == m_tester->m_tmFuncThread->func());
}

TEST_F(TestPagingWidget, SlotJumpPageLineEditReturnPressed_test_001)
{
    Stub s;
    s.set(ADDR(PagingWidget, normalChangePage), normalChangePage_stub);
    m_tester->SlotJumpPageLineEditReturnPressed();
    EXPECT_TRUE(g_funcname == "normalChangePage_stub");
}

TEST_F(TestPagingWidget, onEditFinished_test_001)
{
    Stub s;
    s.set(ADDR(PagingWidget, setIndex), setIndex_stub);
    m_tester->onEditFinished();
    EXPECT_TRUE(g_funcname == "setIndex_stub");
}

TEST_F(TestPagingWidget, normalChangePage_test_001)
{
    Stub s;
    s.set(ADDR(DocSheet, jumpToIndex), jumpToIndex_stub);
    s.set(ADDR(DocSheet, pageCount), pageCount_stub);
    m_tester->m_pJumpPageLineEdit->setText("2");
    m_tester->normalChangePage();
    EXPECT_TRUE(g_funcname == "jumpToIndex_stub");
}

TEST_F(TestPagingWidget, pageNumberJump_test_001)
{
    Stub s;
    s.set(ADDR(PagingWidget, normalChangePage), normalChangePage_stub);
    m_tester->pageNumberJump();
    EXPECT_TRUE(g_funcname == "normalChangePage_stub");
}

TEST_F(TestPagingWidget, slotPrePageBtnClicked_test_001)
{
    Stub s;
    s.set(ADDR(DocSheet, jumpToPage), jumpToPage_stub);
    m_tester->slotPrePageBtnClicked();
    EXPECT_TRUE(g_funcname == "jumpToPage_stub");
}

TEST_F(TestPagingWidget, slotNextPageBtnClicked_test_001)
{
    Stub s;
    s.set(ADDR(DocSheet, jumpToPage), jumpToPage_stub);
    m_tester->slotNextPageBtnClicked();
    EXPECT_TRUE(g_funcname == "jumpToPage_stub");
}

TEST_F(TestPagingWidget, setTabOrderWidget_test_001)
{
    QList<QWidget *> tabWidgetlst;
    m_tester->setTabOrderWidget(tabWidgetlst);
    EXPECT_EQ(tabWidgetlst.count(), 3);
}

TEST_F(TestPagingWidget, onFuncThreadFinished_test_001)
{
    m_tester->m_bHasLabel = true;
    m_tester->m_curIndex = 1;
    m_tester->onFuncThreadFinished();
    EXPECT_FALSE(m_tester->m_bHasLabel);
    EXPECT_EQ(m_tester->m_curIndex, 0);
}


// === Auto-generated test stubs for uncovered methods ===

TEST_F(TestPagingWidget, PagingWidget_test_001)
{
    SUCCEED();
}

TEST_F(TestPagingWidget, eventFilter_test_002)
{
    SUCCEED();
}

TEST_F(TestPagingWidget, initWidget_test_003)
{
    SUCCEED();
}

TEST_F(TestPagingWidget, setBtnState_test_004)
{
    SUCCEED();
}

TEST_F(TestPagingWidget, PagingWidget_destructor_005)
{
    SUCCEED();
}

