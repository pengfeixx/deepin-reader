/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     zhangsong<zhangsong@uniontech.com>
*
* Maintainer: zhangsong<zhangsong@uniontech.com>
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
#ifndef DocSheet_H
#define DocSheet_H

#include <DSplitter>
#include "document/Model.h"

class SheetSidebar;
class SlideWidget;
struct SheetOperation {
    Dr::LayoutMode layoutMode   = Dr::SinglePageMode;
    Dr::MouseShape mouseShape   = Dr::MouseShapeNormal;
    Dr::ScaleMode scaleMode     = Dr::ScaleFactorMode;
    Dr::Rotation rotation       = Dr::RotateBy0;
    qreal scaleFactor           = 1.0;
    bool sidebarVisible         = false;
    int  sidebarIndex           = 0;
    int  currentPage            = 1;
};

class SheetBrowser;
class DocSheet : public Dtk::Widget::DSplitter
{
    Q_OBJECT
    Q_DISABLE_COPY(DocSheet)

    friend class SheetSidebar;
    friend class Database;
public:
    explicit DocSheet(Dr::FileType fileType, QString filePath, QWidget *parent = nullptr);

    ~DocSheet();

public:
    static QImage firstThumbnail(const QString &filePath);

    static bool existFileChanged();

    static QUuid getUuid(DocSheet *);

    static DocSheet *getSheet(QString uuid);

    static QMap<QString, DocSheet *> g_map;

public:
    void initOperationData(const SheetOperation &opera);

    bool isOpen();

    void openFile();

    bool openFileExec();

    int pagesNumber();

    int currentPage();      //从1开始

    int currentIndex();     //从0开始

    void jumpToIndex(int index);

    void jumpToPage(int page);

    void jumpToFirstPage();

    void jumpToLastPage();

    void jumpToNextPage();

    void jumpToPrevPage();

    deepin_reader::Outline outline();

    void jumpToOutline(const qreal  &left, const qreal &top, unsigned int page);

    void jumpToHighLight(deepin_reader::Annotation *annotation);

    void rotateLeft();

    void rotateRight();

    void setBookMark(int index, int state);

    void setBookMarks(const QList<int> &indexlst, int state);

    void setLayoutMode(Dr::LayoutMode mode);

    void setScaleMode(Dr::ScaleMode mode);

    void setScaleFactor(qreal scaleFactor);     //base is 1 ;设置后自动取消自适应

    void setMouseShape(Dr::MouseShape shape);

    void setAnnotationInserting(bool inserting);

    void setState();

    void openMagnifier();

    void closeMagnifier();

    bool magnifierOpened();

    bool fileChanged();

    bool saveData();

    bool saveAsData(QString filePath);

    void handleSearch();

    void stopSearch();

    void copySelectedText();                    //复制选中文字

    void highlightSelectedText();                   //高亮选中文字

    void addSelectedTextHightlightAnnotation();       //对选中文字添加高亮注释

    bool getImage(int index, QImage &image, double width, double height, Qt::AspectRatioMode mode = Qt::IgnoreAspectRatio);

    void defaultFocus();

    QList<deepin_reader::Annotation *> annotations();

    bool removeAnnotation(deepin_reader::Annotation *annotation);

    bool removeAnnotations(const QList<deepin_reader::Annotation *> &annotations);

    QList<qreal> scaleFactorList();

    qreal maxScaleFactor();

    QString filter();

    QSet<int> getBookMarkList() const;

    SheetOperation operation();

    Dr::FileType fileType();

    QString filePath();

    bool hasBookMark(int index);

    void zoomin();  //放大一级

    void zoomout();

    bool sideBarVisible();

    void setSidebarVisible(bool isVisible, bool notify = true);

    void handleOpenSuccess();

    void openSlide();

    void closeSlide();

    bool slideOpened();

    bool isFullScreen();

    void openFullScreen();

    void closeFullScreen();

public slots:
    void showTips(const QString &tips, int iconIndex = 0);

public:
    int label2pagenum(QString label);

    bool haslabel();

    void docBasicInfo(deepin_reader::FileInfo &info);

    QString pagenum2label(int index);

signals:
    void sigFindOperation(const int &);

signals:
    void sigFileChanged(DocSheet *);    //被修改了 书签 笔记

    void sigOpened(DocSheet *, bool);

private slots:
    void onBrowserPageChanged(int page);

    void onBrowserSizeChanged();

    void onBrowserWheelUp();

    void onBrowserWheelDown();

    void onBrowserPageFirst();

    void onBrowserPagePrev();

    void onBrowserPageNext();

    void onBrowserPageLast();

    void onBrowserBookmark(int index, bool state);

    void onBrowserOperaAnnotation(int, deepin_reader::Annotation *);

private:
    SheetOperation  m_operation;
    QSet<int>       m_bookmarks;

    SheetSidebar   *m_sidebar = nullptr;
    SheetBrowser   *m_browser = nullptr;

    QString         m_filePath;
    Dr::FileType    m_fileType;
    QString         m_uuid;

    bool m_fileChanged = false;

};

#endif // DocSheet_H
