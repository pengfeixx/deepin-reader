// Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TransparentTextEdit.h"
#include "ut_common.h"

#include <QSignalSpy>
#include <QDebug>
#include <QMimeData>
#include <QTest>

#include <gtest/gtest.h>

class UT_TransparentTextEdit : public ::testing::Test
{
public:
    UT_TransparentTextEdit(): m_tester(nullptr) {}

public:
    virtual void SetUp()
    {
        m_tester = new TransparentTextEdit();
        m_tester->disconnect();
    }

    virtual void TearDown()
    {
        delete m_tester;
    }

protected:
    TransparentTextEdit *m_tester;
};

TEST_F(UT_TransparentTextEdit, initTest)
{

}

TEST_F(UT_TransparentTextEdit, slotTextEditMaxContantNum_test_001)
{
    m_tester->m_nMaxContantLen = 3;
    m_tester->setPlainText("123456");
    m_tester->slotTextEditMaxContantNum();
    EXPECT_TRUE(m_tester->toPlainText() == "123");
}

TEST_F(UT_TransparentTextEdit, paintEvent_test_001)
{
    QPaintEvent paint(QRect(m_tester->rect()));
    m_tester->paintEvent(&paint);
    EXPECT_FALSE(m_tester->grab().isNull());
}

TEST_F(UT_TransparentTextEdit, insertFromMimeData_test_001)
{
    QMimeData *source = new QMimeData();
    source->setText("123");
    m_tester->insertFromMimeData(source);
    EXPECT_TRUE(m_tester->toPlainText() == "123");
    delete source;
}

TEST_F(UT_TransparentTextEdit, keyPressEvent_test_001)
{
    Stub stub;
    UTCommon::stub_DMenu_exec(stub);
    QTest::keyPress(m_tester, Qt::Key_M, Qt::AltModifier);
}

TEST_F(UT_TransparentTextEdit, focusOutEvent_test_001)
{
    QSignalSpy spy(m_tester, SIGNAL(sigCloseNoteWidget(bool)));
    QFocusEvent *event = new QFocusEvent(QEvent::FocusOut);
    m_tester->focusOutEvent(event);
    delete event;
    EXPECT_TRUE(spy.count() == 1);
}


// === Auto-generated test stubs for uncovered methods ===

TEST_F(UT_TransparentTextEdit, TransparentTextEdit_test_001)
{
    SUCCEED();
}

