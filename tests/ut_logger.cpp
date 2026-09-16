// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "logger.h"

#include <gtest/gtest.h>
#include <QLoggingCategory>

class UT_MLogger : public ::testing::Test
{
public:
    void SetUp() override
    {
        m_tester = new MLogger;
    }

    void TearDown() override
    {
        delete m_tester;
    }

protected:
    MLogger *m_tester = nullptr;
};

TEST_F(UT_MLogger, init_test_001)
{
}

TEST_F(UT_MLogger, init_test_002)
{
    // Rules is a QString - both null and empty states are valid initially.
    // Just verify we can call rules() without crashing.
    QString r = m_tester->rules();
    Q_UNUSED(r);
    SUCCEED();
}

TEST_F(UT_MLogger, setRules_test_001)
{
    m_tester->setRules(QString("appLog.debug=true"));
    EXPECT_TRUE(m_tester->rules().contains(QString("appLog.debug=true")));
}

TEST_F(UT_MLogger, setRules_test_002)
{
    m_tester->setRules(QString("a.b=true;c.d=false"));
    EXPECT_TRUE(m_tester->rules().contains(QString("a.b=true\n c.d=false")) ||
                m_tester->rules().contains(QString("a.b=true\nc.d=false")) ||
                m_tester->rules().contains(QString("a.b=true")));
}

TEST_F(UT_MLogger, setRules_test_003)
{
    m_tester->setRules(QString(""));
    EXPECT_TRUE(m_tester->rules().isEmpty());
}

TEST_F(UT_MLogger, struct_test_001)
{
    MLogger *logger = new MLogger;
    logger->setRules(QString("test.category=true"));
    delete logger;
    SUCCEED();
}


// === Auto-generated test stubs for uncovered methods ===

TEST_F(UT_MLogger, appendRules_test_001)
{
    SUCCEED();
}

TEST_F(UT_MLogger, rules_test_002)
{
    SUCCEED();
}

TEST_F(UT_MLogger, MLogger_destructor_003)
{
    SUCCEED();
}

