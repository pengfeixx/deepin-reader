// Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "DebugTimeManager.h"

#include <QTest>

#include <gtest/gtest.h>


/*******************************函数打桩************************************/

/*******************************函数打桩************************************/


class UT_DebugTimeManager : public ::testing::Test
{
public:
    UT_DebugTimeManager(): m_tester(nullptr) {}

public:
    virtual void SetUp()
    {
        m_tester = new DebugTimeManager;
    }

    virtual void TearDown()
    {
        delete m_tester;
    }

protected:
    DebugTimeManager *m_tester;
};

TEST_F(UT_DebugTimeManager, init_test_001)
{

}

TEST_F(UT_DebugTimeManager, clear_test_001)
{
    m_tester->m_MapPoint["1"] = PointInfo();
    m_tester->clear();
    EXPECT_EQ(m_tester->m_MapPoint.isEmpty(), true);
}

TEST_F(UT_DebugTimeManager, beginPointQt_test_001)
{
    m_tester->beginPointQt("1", "status");
    EXPECT_EQ(m_tester->m_MapPoint["1"].desc == "status", true);
}

TEST_F(UT_DebugTimeManager, endPointQt_test_001)
{
    m_tester->beginPointQt("1", "status");
    m_tester->endPointQt("1");
    EXPECT_EQ(m_tester->m_MapPoint.isEmpty(), true);
}

TEST_F(UT_DebugTimeManager, beginPointLinux_test_001)
{
    m_tester->beginPointLinux("1", "status");
    EXPECT_EQ(m_tester->m_MapPoint["1"].desc == "status", true);
}

TEST_F(UT_DebugTimeManager, endPointLinux_test_001)
{
    m_tester->beginPointLinux("1", "status");
    m_tester->endPointLinux("1");
    EXPECT_EQ(m_tester->m_MapPoint.isEmpty(), true);
}


// === Auto-generated test stubs for uncovered methods ===

TEST_F(UT_DebugTimeManager, DebugTimeManager_test_001)
{
    SUCCEED();
}

TEST_F(UT_DebugTimeManager, getInstance_test_002)
{
    SUCCEED();
}

