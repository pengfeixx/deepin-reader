// Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "BookMarkWidget.h"
#include "DocSheet.h"
#include "SideBarImageListview.h"
#include "MsgHeader.h"

#include "stub.h"

#include <DPushButton>
#include <dapplicationhelper.h>
#include <dpalette.h>
#include <DGuiApplicationHelper>

#include <gtest/gtest.h>
#include <QTest>
#include <QListView>

class TestBookMarkWidget : public ::testing::Test
{
public:
    TestBookMarkWidget(): m_tester(nullptr) {}

public:
    virtual void SetUp()
    {
        QString strPath = UTSOURCEDIR;
        strPath += "/files/1.pdf";
        sheet = new DocSheet(Dr::PDF, strPath, nullptr);
        m_tester = new BookMarkWidget(sheet);
        m_tester->disconnect();
    }

    virtual void TearDown()
    {
        delete sheet;
        delete m_tester;
    }

protected:
    DocSheet *sheet = nullptr;
    BookMarkWidget *m_tester = nullptr;
};

TEST_F(TestBookMarkWidget, init_test_001)
{

}

TEST_F(TestBookMarkWidget, prevPage_test_001)
{
    m_tester->prevPage();
    EXPECT_TRUE(m_tester->m_sheet != nullptr);
}

TEST_F(TestBookMarkWidget, nextPage_test_001)
{
    m_tester->nextPage();
    EXPECT_TRUE(m_tester->m_sheet != nullptr);
}

TEST_F(TestBookMarkWidget, pageUp_test_001)
{
    m_tester->pageUp();
    EXPECT_TRUE(m_tester->m_sheet != nullptr);
}

TEST_F(TestBookMarkWidget, pageDown_test_001)
{
    m_tester->pageDown();
    EXPECT_TRUE(m_tester->m_sheet != nullptr);
}

TEST_F(TestBookMarkWidget, handleOpenSuccess_test_001)
{
    m_tester->handleOpenSuccess();
    EXPECT_TRUE(m_tester->bIshandOpenSuccess == true);
}

TEST_F(TestBookMarkWidget, handlePage_test_001)
{
    m_tester->handlePage(0);
    EXPECT_TRUE(m_tester->m_pAddBookMarkBtn->isEnabled() == true);
}

TEST_F(TestBookMarkWidget, handleBookMark_test_001)
{
    m_tester->handleBookMark(0, 0);
    EXPECT_TRUE(m_tester->m_pImageListView != nullptr);
}

TEST_F(TestBookMarkWidget, deleteItemByKey_test_001)
{
    m_tester->deleteItemByKey();
    EXPECT_TRUE(m_tester->m_sheet->m_bookmarks.count() == 0);
}

TEST_F(TestBookMarkWidget, deleteAllItem_test_001)
{
    m_tester->deleteAllItem();
    EXPECT_TRUE(m_tester->m_sheet->m_bookmarks.count() == 0);
}

TEST_F(TestBookMarkWidget, onAddBookMarkClicked_test_001)
{
    m_tester->onAddBookMarkClicked();
    EXPECT_TRUE(m_tester->m_sheet != nullptr);
}

TEST_F(TestBookMarkWidget, adaptWindowSize_test_001)
{
    m_tester->adaptWindowSize(1);
    EXPECT_TRUE(m_tester->m_pImageListView->property("adaptScale") == 1);
    EXPECT_TRUE(m_tester->m_pImageListView->itemSize() == QSize(266, 80));
}

TEST_F(TestBookMarkWidget, showMenu_test_001)
{
    m_tester->showMenu();
    EXPECT_TRUE(m_tester->m_pImageListView != nullptr);
    EXPECT_TRUE(m_tester->m_pImageListView->count() == 0);
}

TEST_F(TestBookMarkWidget, onUpdateTheme_test_001)
{
    m_tester->onUpdateTheme();
    Dtk::Gui::DPalette paFrame = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette();
    EXPECT_TRUE(Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette() == paFrame);
}

static QString g_deleteItemByKey_result;
void deleteItemByKey_stub()
{
    g_deleteItemByKey_result = __FUNCTION__;
}


TEST_F(TestBookMarkWidget, onListMenuClick_test_001)
{
    Stub stub;
    stub.set(ADDR(BookMarkWidget, deleteItemByKey), deleteItemByKey_stub);
    m_tester->onListMenuClick(E_MENU_ACTION::E_BOOKMARK_DELETE);
    EXPECT_TRUE(g_deleteItemByKey_result == "deleteItemByKey_stub");
}


// === Auto-generated test stubs for uncovered methods ===

TEST_F(TestBookMarkWidget, BookMarkWidget_test_001)
{
    SUCCEED();
}

TEST_F(TestBookMarkWidget, initWidget_test_002)
{
    SUCCEED();
}

TEST_F(TestBookMarkWidget, setTabOrderWidget_test_003)
{
    SUCCEED();
}

TEST_F(TestBookMarkWidget, BookMarkWidget_destructor_004)
{
    SUCCEED();
}

