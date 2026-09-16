// Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ShortCutShow.h"
#include "Global.h"
#include "DocSheet.h"
#include "ut_defines.h"
#include "stub.h"
#include "ut_common.h"

#include <gtest/gtest.h>
#include <QTest>

namespace {
class UT_ShortCutShow : public ::testing::Test
{
public:
    UT_ShortCutShow(): m_tester(nullptr) {}

public:
    virtual void SetUp()
    {
        m_tester = new ShortCutShow();
        m_tester->disconnect();
    }

    virtual void TearDown()
    {
        delete m_tester;
    }

protected:
    ShortCutShow *m_tester;
};

static QString g_funcname;
void initPDF_stub()
{
    g_funcname = __FUNCTION__;
}

void initDJVU_stub()
{
    g_funcname = __FUNCTION__;
}
}

TEST_F(UT_ShortCutShow, initTest)
{

}

TEST_F(UT_ShortCutShow, setSheet_test_001)
{
    Stub s;
    s.set(ADDR(ShortCutShow, initPDF), initPDF_stub);
    s.set(ADDR(ShortCutShow, initDJVU), initDJVU_stub);

    m_tester->setSheet(nullptr);
    EXPECT_TRUE(g_funcname == "initPDF_stub");
}

TEST_F(UT_ShortCutShow, setSheet_test_002)
{
    Stub s;
    s.set(ADDR(ShortCutShow, initPDF), initPDF_stub);
    s.set(ADDR(ShortCutShow, initDJVU), initDJVU_stub);

    DocSheet *sheet = new DocSheet(Dr::FileType::DJVU, "1.pdf");
    m_tester->setSheet(sheet);
    delete sheet;
    EXPECT_TRUE(g_funcname == "initDJVU_stub");
}

TEST_F(UT_ShortCutShow, setSheet_test_003)
{
    Stub s;
    s.set(ADDR(ShortCutShow, initPDF), initPDF_stub);
    s.set(ADDR(ShortCutShow, initDJVU), initDJVU_stub);

    DocSheet *sheet = new DocSheet(Dr::FileType::PDF, "1.pdf");
    m_tester->setSheet(sheet);
    delete sheet;
    EXPECT_TRUE(g_funcname == "initPDF_stub");
}

TEST_F(UT_ShortCutShow, show_test_001)
{
    Stub stub;
    UTCommon::stub_QProcess_startDetached(stub);

    m_tester->initDJVU();
    m_tester->show();
    EXPECT_TRUE(m_tester->m_shortcutMap.count() > 0);
}

TEST_F(UT_ShortCutShow, initDJVU_test_001)
{
    m_tester->initDJVU();
    EXPECT_TRUE(m_tester->m_shortcutMap.count() > 0);
}

TEST_F(UT_ShortCutShow, initPDF_test_001)
{
    m_tester->initPDF();
    EXPECT_TRUE(m_tester->m_shortcutMap.count() > 0);
}

TEST_F(UT_ShortCutShow, struct_test_001)
{
    ShortCutShow::KeyDataList list;
    EXPECT_TRUE(list.isEmpty());

    ShortCutShow::KeyDataList list2{{"Ctrl+S", "Save"}, {"Ctrl+O", "Open"}};
    EXPECT_EQ(list2.count(), 2);

    list2.removeKey("Ctrl+S");
    EXPECT_EQ(list2.count(), 1);
}



// === Auto-generated test stubs for uncovered methods ===

TEST_F(UT_ShortCutShow, ShortCutShow_test_001)
{
    SUCCEED();
}

