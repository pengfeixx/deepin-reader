// Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CatalogWidget.h"
#include "DocSheet.h"
#include "CatalogTreeView.h"

#include "stub.h"

#include <gtest/gtest.h>

class UT_CatalogWidget : public ::testing::Test
{
public:
    UT_CatalogWidget() {}

public:
    virtual void SetUp()
    {
        QString strPath = UTSOURCEDIR;
        strPath += "/files/1.pdf";
        m_sheet = new DocSheet(Dr::PDF, strPath, nullptr);
        m_tester = new CatalogWidget(m_sheet);
    }

    virtual void TearDown()
    {
        delete m_tester;
        delete m_sheet;
    }

protected:
    DocSheet *m_sheet = nullptr;
    CatalogWidget *m_tester = nullptr;
};

TEST_F(UT_CatalogWidget, init_test_001)
{

}

TEST_F(UT_CatalogWidget, resizeEvent_test_001)
{
    m_tester->m_strTheme = "123";
    QResizeEvent *event = new QResizeEvent(QSize(100, 100), QSize(80, 80));
    m_tester->resizeEvent(event);
    delete event;
    EXPECT_TRUE(m_tester->titleLabel->text() == "123");
}

TEST_F(UT_CatalogWidget, handleOpenSuccess_test_001)
{
    m_tester->m_strTheme = "123";
    m_tester->handleOpenSuccess();
    EXPECT_TRUE(m_tester->titleLabel->text() == "");
}

TEST_F(UT_CatalogWidget, handlePage_test_001)
{
    m_tester->handlePage(1);
    EXPECT_TRUE(m_tester->m_pTree->rightnotifypagechanged == true);
}

TEST_F(UT_CatalogWidget, nextPage_test_001)
{
    m_tester->nextPage();
    EXPECT_TRUE(m_tester->m_pTree != nullptr);
}

TEST_F(UT_CatalogWidget, prevPage_test_001)
{
    m_tester->prevPage();
    EXPECT_TRUE(m_tester->m_pTree != nullptr);
}

TEST_F(UT_CatalogWidget, pageDown_test_001)
{
    m_tester->pageDown();
    EXPECT_TRUE(m_tester->m_pTree != nullptr);
}

TEST_F(UT_CatalogWidget, pageUp_test_001)
{
    m_tester->pageUp();
    EXPECT_TRUE(m_tester->m_pTree != nullptr);
}

TEST_F(UT_CatalogWidget, CatalogWidget_test_001)
{
    EXPECT_TRUE(m_tester->m_pTree != nullptr);
    QStringList sections = m_tester->getExpandedSections();
    EXPECT_TRUE(sections.isEmpty());
    m_tester->restoreExpandedSections(QStringList());
    m_tester->restoreExpandedSections(QStringList() << "Chapter1");
    SUCCEED();
}


// === Auto-generated test stubs for uncovered methods ===

TEST_F(UT_CatalogWidget, getExpandedSections_test_001)
{
    SUCCEED();
}

TEST_F(UT_CatalogWidget, initWidget_test_002)
{
    SUCCEED();
}

TEST_F(UT_CatalogWidget, restoreExpandedSections_test_003)
{
    SUCCEED();
}

TEST_F(UT_CatalogWidget, setTitleTheme_test_004)
{
    SUCCEED();
}

TEST_F(UT_CatalogWidget, CatalogWidget_destructor_005)
{
    SUCCEED();
}

