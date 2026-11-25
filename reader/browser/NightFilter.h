// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NIGHTFILTER_H
#define NIGHTFILTER_H

#include <QImage>

/**
 * @brief The NightFilter namespace
 * 夜间模式滤镜:
 *   文字/背景按 CIELAB L* 反转(感知均匀的明度反转,保留色度 a/b),
 *   原深色像素反转后提纯白,保证文字/抗锯齿边缘对比度。
 */
namespace NightFilter {

struct Options {
    qreal minLightness = 11.0;       // 反转后 L* 下限(约 #1E1E1E,防整页全黑刺眼)
    qreal boostSourceBelow = 27.0;   // 原 L* 低于此值(约 #404040)的像素反转后提纯白,保文字对比度
};

/**
 * @brief applyPage 对整页应用夜间滤镜(可在非 UI 线程调用,无锁)
 * @param src 渲染输出(ARGB32 / RGB32)
 * @param opt 滤镜参数
 * @return 处理后的图像(格式与输入一致)
 */
QImage applyPage(const QImage &src, const Options &opt = Options());

/** sRGB(0..255)→ CIELAB(D65),单像素 */
void srgbToLab(int r, int g, int b, double &L, double &a, double &bb);

/** CIELAB(D65)→ sRGB(0..255),越界自动钳制 */
void labToSrgb(double L, double a, double bb, int &r, int &g, int &b);

} // namespace NightFilter

#endif // NIGHTFILTER_H
