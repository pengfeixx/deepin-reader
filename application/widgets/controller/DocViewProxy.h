/*
 * Copyright (C) 2019 ~ 2020 UOS Technology Co., Ltd.
 *
 * Author:     wangzhxiaun
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DOCVIEWPROXY_H
#define DOCVIEWPROXY_H

#include <QObject>
#include "MsgHeader.h"

/**
 *  @brief  文档 缩放\单双页\旋转  操作
 */

class DocummentProxy;

class DocViewProxy : public QObject
{
    Q_OBJECT
public:
    explicit DocViewProxy(QObject *parent = nullptr);

public:
    void setProxy(DocummentProxy *proxy);

    void onSetWidgetAdapt();
    void OnSetViewHit(const QString &msgContent);
    void OnSetViewRotate(const QString &msgConent);
    void OnSetViewScale(const QString &msgConent);
    void OnSetViewChange(const QString &msgContent);
    void setScaleRotateViewModeAndShow();

    void setAdapteState(const int &nAdapteState);
    void setDoubleShow(const int &nDoubleShow);
    void setRotateType(const int &rotateType);
    void setScale(const int &nScale);
    void setWidth(const int &nWidth);
    void setHeight(const int &nHeight);

private:
    void notifyMsg(const int &msgType, const QString &msgContent);

private:
    DocummentProxy      *m_pProxy = nullptr;

    int                 m_nAdapteState = NO_ADAPTE_State;       //  当前自适应状态

    int                 m_nDoubleShow = false;
    int                 m_rotateType = 0;            // 旋转类型(后台所需旋转类型)
    int                 m_nScale = 0;

    int                 m_nWidth = 0;
    int                 m_nHeight = 0;
};

#endif // DOCVIEWPROXY_H
