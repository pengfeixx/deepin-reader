// Copyright (C) 2019-2026 ~ 2020 UnionTech Software Technology Co.,Ltd.
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ReaderImageThreadPoolManager.h"
#include "DocSheet.h"

#include "ut_common.h"

#include "stub.h"

#include <gtest/gtest.h>
#include <QPixmap>
#include <QImage>
#include <QTest>

class UT_ReadImageTask : public ::testing::Test
{
public:
    UT_ReadImageTask(): m_tester(nullptr) {}

public:
    virtual void SetUp()
    {
        m_tester = new ReadImageTask();
    }

    virtual void TearDown()
    {
        delete m_tester;
    }

protected:
    ReadImageTask *m_tester;
};

TEST_F(UT_ReadImageTask, init_test_001)
{

}

TEST_F(UT_ReadImageTask, addgetDocImageTask_test_001)
{
    ReaderImageParam_t readImageParam;
    readImageParam.pageIndex = 1;
    m_tester->addgetDocImageTask(readImageParam);
    EXPECT_TRUE(m_tester->m_docParam.pageIndex == 1);
}

TEST_F(UT_ReadImageTask, setThreadPoolManager_test_001)
{
    QObject *object = new QObject();
    m_tester->setThreadPoolManager(object);
    EXPECT_TRUE(m_tester->m_threadpoolManager == object);
    delete object;
}

TEST_F(UT_ReadImageTask, run_test_001)
{
    // run() with no sheet set should be safe (no crash, no-op)
    m_tester->run();
    SUCCEED();
}

class UT_ReaderImageThreadPoolManager : public ::testing::Test
{
public:
    void SetUp() override
    {
        m_tester = ReaderImageThreadPoolManager::getInstance();
    }

    void TearDown() override
    {
    }

protected:
    ReaderImageThreadPoolManager *m_tester;
};

TEST_F(UT_ReaderImageThreadPoolManager, init_test_001)
{
    EXPECT_NE(m_tester, nullptr);
}

TEST_F(UT_ReaderImageThreadPoolManager, getInstance_test_001)
{
    ReaderImageThreadPoolManager *instance1 = ReaderImageThreadPoolManager::getInstance();
    ReaderImageThreadPoolManager *instance2 = ReaderImageThreadPoolManager::getInstance();
    EXPECT_EQ(instance1, instance2);
}

TEST_F(UT_ReaderImageThreadPoolManager, getImageForDocSheet_test_001)
{
    QPixmap pix = m_tester->getImageForDocSheet(nullptr, 0);
    EXPECT_TRUE(pix.isNull());
}

TEST_F(UT_ReaderImageThreadPoolManager, onDocProxyDestroyed_test_001)
{
    QObject obj;
    m_tester->onDocProxyDestroyed(&obj);
    SUCCEED();
}

TEST_F(UT_ReaderImageThreadPoolManager, onReceiverDestroyed_test_001)
{
    QObject obj;
    m_tester->onReceiverDestroyed(&obj);
    SUCCEED();
}

TEST_F(UT_ReaderImageThreadPoolManager, UT_ReaderImageParam_t_operators)
{
    ReaderImageParam_t a, b;
    a.pageIndex = 1;
    a.maxPixel = 100;
    b.pageIndex = 1;
    b.maxPixel = 100;
    EXPECT_TRUE(a == b);

    b.pageIndex = 2;
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(a > b);

    a.pageIndex = 3;
    EXPECT_TRUE(a > b);
}

static int pageCount_stub_pool()
{
    return 2;
}

TEST_F(UT_ReaderImageThreadPoolManager, setImageForDocSheet_test_001)
{
    QString strPath = UTSOURCEDIR;
    strPath += "/files/1.pdf";
    DocSheet *sheet = new DocSheet(Dr::PDF, strPath, nullptr);

    Stub s;
    s.set(ADDR(DocSheet, pageCount), pageCount_stub_pool);

    // First initialize the sheet in the map via addgetDocImageTask
    ReaderImageParam_t param;
    param.pageIndex = 0;
    param.sheet = sheet;
    param.receiver = new QObject();
    QObject::connect(sheet, &QObject::destroyed, m_tester, &ReaderImageThreadPoolManager::onDocProxyDestroyed);

    QPixmap pix(10, 10);
    m_tester->setImageForDocSheet(sheet, 0, pix);
    delete param.receiver;
    delete sheet;
    SUCCEED();
}

TEST_F(UT_ReaderImageThreadPoolManager, onTaskFinished_test_001)
{
    QString strPath = UTSOURCEDIR;
    strPath += "/files/1.pdf";
    DocSheet *sheet = new DocSheet(Dr::PDF, strPath, nullptr);

    ReaderImageParam_t param;
    param.pageIndex = 0;
    param.sheet = sheet;
    param.receiver = new QObject();
    param.slotFun = "dummySlot";

    QObject::connect(sheet, &QObject::destroyed, m_tester, &ReaderImageThreadPoolManager::onDocProxyDestroyed);

    QImage img(10, 10, QImage::Format_ARGB32);
    m_tester->onTaskFinished(param, img);

    delete param.receiver;
    delete sheet;
    SUCCEED();
}

TEST_F(UT_ReaderImageThreadPoolManager, addgetDocImageTask_test_001)
{
    QString strPath = UTSOURCEDIR;
    strPath += "/files/1.pdf";
    DocSheet *sheet = new DocSheet(Dr::PDF, strPath, nullptr);

    Stub s;
    s.set(ADDR(DocSheet, pageCount), pageCount_stub_pool);

    QObject *receiver = new QObject();
    ReaderImageParam_t param;
    param.pageIndex = 0;
    param.sheet = sheet;
    param.receiver = receiver;
    param.slotFun = "dummySlot";

    QObject::connect(sheet, &QObject::destroyed, m_tester, &ReaderImageThreadPoolManager::onDocProxyDestroyed);

    m_tester->addgetDocImageTask(param);

    // Calling again with same params should be skipped (duplicate)
    m_tester->addgetDocImageTask(param);

    // Wait briefly to allow task to run
    QTest::qWait(50);

    delete receiver;
    delete sheet;
    SUCCEED();
}
