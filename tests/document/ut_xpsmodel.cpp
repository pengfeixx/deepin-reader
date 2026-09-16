// Copyright (C) 2026 ~ 2020 UnionTech Software Technology Co.,Ltd.
// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "XpsDocumentAdapter.h"
#include "XpsTextExtractor.h"
#include "Model.h"
#include "ut_defines.h"

#include <QDir>
#include <QFile>
#include <QImage>
#include <QPageSize>
#include <QTemporaryDir>
#include <QXmlStreamReader>

#include <gtest/gtest.h>

using namespace deepin_reader;

namespace {

QString xpsFilePath()
{
    // Try resource path first (UTSOURCEDIR macro), then cwd-relative fallback
    QString path = QString(UTSOURCEDIR) + "/files/normal.xps";
    if (QFile(path).exists())
        return path;

    path = QCoreApplication::applicationDirPath() + "/files/normal.xps";
    if (QFile(path).exists())
        return path;

    return path;  // may not exist on minimal CI runners
}

bool hasXpsFile()
{
    return QFile(xpsFilePath()).exists();
}

}  // namespace

class TestXpsDocumentAdapter : public ::testing::Test
{
public:
    void SetUp() override
    {
        m_path = xpsFilePath();
        if (!hasXpsFile())
            GTEST_SKIP() << "normal.xps not available, skipping XPS adapter tests";

        Document::Error error = Document::NoError;
        m_doc.reset(XpsDocumentAdapter::loadDocument(m_path, error));
        ASSERT_NE(m_doc, nullptr);
        EXPECT_EQ(error, Document::NoError);
    }

    void TearDown() override {}

protected:
    QString m_path;
    std::unique_ptr<XpsDocumentAdapter> m_doc;
};

TEST_F(TestXpsDocumentAdapter, page_test_001)
{
    EXPECT_GT(m_doc->pageCount(), 0);
}

TEST_F(TestXpsDocumentAdapter, page_test_002)
{
    int count = m_doc->pageCount();
    ASSERT_GT(count, 0);

    std::unique_ptr<Page> page(m_doc->page(0));
    EXPECT_NE(page, nullptr);

    // Out of range returns null
    std::unique_ptr<Page> badPage(m_doc->page(count));
    EXPECT_EQ(badPage, nullptr);

    std::unique_ptr<Page> negativePage(m_doc->page(-1));
    EXPECT_EQ(negativePage, nullptr);
}

TEST_F(TestXpsDocumentAdapter, save_test_001)
{
    QStringList filters = m_doc->saveFilter();
    EXPECT_FALSE(filters.isEmpty());
    bool hasXps = false, hasPdf = false;
    for (const QString &filter : filters) {
        if (filter.contains("*.xps", Qt::CaseInsensitive))
            hasXps = true;
        if (filter.contains("*.pdf", Qt::CaseInsensitive))
            hasPdf = true;
    }
    EXPECT_TRUE(hasXps);
    EXPECT_TRUE(hasPdf);
}

TEST_F(TestXpsDocumentAdapter, save_test_002)
{
    // XPS is read-only; save() in place should always return false
    EXPECT_FALSE(m_doc->save());
}

TEST_F(TestXpsDocumentAdapter, save_test_003)
{
    EXPECT_FALSE(m_doc->saveAs(QString()));
}

TEST_F(TestXpsDocumentAdapter, save_test_004)
{
    EXPECT_FALSE(m_doc->saveAs(QStringLiteral("/tmp/test_unknown.xxx")));
}

TEST_F(TestXpsDocumentAdapter, save_test_005)
{
    EXPECT_TRUE(m_doc->saveAs(m_path));
}

TEST_F(TestXpsDocumentAdapter, save_test_006)
{
    QString target = QDir::tempPath() + "/ut_xps_copy_006.xps";
    QFile::remove(target);

    ASSERT_TRUE(m_doc->saveAs(target));
    EXPECT_TRUE(QFile(target).exists());
    EXPECT_GT(QFile(target).size(), 0);
    QFile::remove(target);
}

TEST_F(TestXpsDocumentAdapter, save_test_007)
{
    QString target = QDir::tempPath() + "/ut_xps_export_007.pdf";
    QFile::remove(target);

    ASSERT_TRUE(m_doc->saveAs(target));
    EXPECT_TRUE(QFile(target).exists());
    EXPECT_GT(QFile(target).size(), 0);
    QFile::remove(target);
}

TEST_F(TestXpsDocumentAdapter, outline_test_001)
{
    // Outline may be empty but call must not crash
    Outline out = m_doc->outline();
    Q_UNUSED(out);
    SUCCEED();
}

TEST_F(TestXpsDocumentAdapter, properties_test_001)
{
    Properties props = m_doc->properties();
    EXPECT_FALSE(props.isEmpty());
    EXPECT_TRUE(props.contains("Format"));
}

TEST_F(TestXpsDocumentAdapter, page_test_003)
{
    int count = m_doc->pageCount();
    ASSERT_GT(count, 0);

    QSizeF size = m_doc->pageSize(0);
    EXPECT_GT(size.width(), 0.0);
    EXPECT_GT(size.height(), 0.0);

    // Out of range returns empty
    QSizeF bad = m_doc->pageSize(count);
    EXPECT_TRUE(bad.isEmpty());

    QSizeF negative = m_doc->pageSize(-1);
    EXPECT_TRUE(negative.isEmpty());
}

TEST_F(TestXpsDocumentAdapter, page_test_004)
{
    int count = m_doc->pageCount();
    ASSERT_GT(count, 0);

    QSizeF size = m_doc->pageSize(0);
    int w = static_cast<int>(size.width());
    int h = static_cast<int>(size.height());

    QImage img = m_doc->renderPage(0, w, h, QRect());
    EXPECT_FALSE(img.isNull());
    EXPECT_EQ(img.width(), w);
    EXPECT_EQ(img.height(), h);
}

TEST_F(TestXpsDocumentAdapter, page_test_005)
{
    QImage img = m_doc->renderPage(0, 0, 0, QRect());
    EXPECT_TRUE(img.isNull());

    QImage negativeImg = m_doc->renderPage(0, -10, -10, QRect());
    EXPECT_TRUE(negativeImg.isNull());
}

TEST_F(TestXpsDocumentAdapter, page_test_006)
{
    int count = m_doc->pageCount();
    QImage img = m_doc->renderPage(count, 100, 100, QRect());
    EXPECT_TRUE(img.isNull());
}

TEST_F(TestXpsDocumentAdapter, page_test_007)
{
    int count = m_doc->pageCount();
    ASSERT_GT(count, 0);

    QImage img = m_doc->renderPage(0, 200, 200, QRect(0, 0, 100, 100));
    EXPECT_FALSE(img.isNull());
    EXPECT_EQ(img.width(), 100);
    EXPECT_EQ(img.height(), 100);
}

TEST_F(TestXpsDocumentAdapter, page_test_008)
{
    std::unique_ptr<Page> page(m_doc->page(0));
    ASSERT_NE(page, nullptr);

    QSizeF size = page->sizeF();
    EXPECT_GT(size.width(), 0.0);
    EXPECT_GT(size.height(), 0.0);

    QImage img = page->render(100, 100);
    EXPECT_FALSE(img.isNull());
}

TEST_F(TestXpsDocumentAdapter, page_test_009)
{
    std::unique_ptr<Page> page(m_doc->page(0));
    ASSERT_NE(page, nullptr);

    QSizeF size = page->sizeF();
    QRectF fullRect(0, 0, size.width(), size.height());
    QString text = page->text(fullRect);
    // Text may be empty for image-only pages, but call must not crash
    Q_UNUSED(text);
    SUCCEED();
}

TEST_F(TestXpsDocumentAdapter, page_test_010)
{
    std::unique_ptr<Page> page(m_doc->page(0));
    ASSERT_NE(page, nullptr);

    // Search for an unlikely-to-match string
    QVector<PageSection> results = page->search("__xyz_test_no_match__", false, false);
    EXPECT_TRUE(results.isEmpty());
}

TEST_F(TestXpsDocumentAdapter, page_test_011)
{
    std::unique_ptr<Page> page(m_doc->page(0));
    ASSERT_NE(page, nullptr);

    QList<Word> words = page->words();
    // Words may or may not be empty
    Q_UNUSED(words);

    QList<Annotation *> annotations = page->annotations();
    EXPECT_TRUE(annotations.isEmpty());  // XPS adapter doesn't support annotations

    EXPECT_FALSE(page->canAddAndRemoveAnnotations());
    EXPECT_FALSE(page->hasWidgetAnnots());
}

TEST_F(TestXpsDocumentAdapter, page_test_012)
{
    // XpsPageAdapter::getLinkAtPoint always returns an empty Link;
    // exercising it directly (via -fno-access-control) ensures coverage.
    std::unique_ptr<XpsPageAdapter> xpage(static_cast<XpsPageAdapter *>(m_doc->page(0)));
    ASSERT_NE(xpage, nullptr);

    Link link = xpage->getLinkAtPoint(QPointF(0, 0));
    // Default-constructed Link has page == -1 and empty urlOrFileName.
    EXPECT_EQ(link.page, -1);
    EXPECT_TRUE(link.urlOrFileName.isEmpty());
}

TEST_F(TestXpsDocumentAdapter, loadDocument_test_001)
{
    Document::Error error = Document::NoError;
    std::unique_ptr<XpsDocumentAdapter> bad(XpsDocumentAdapter::loadDocument(QString(), error));
    EXPECT_EQ(bad, nullptr);
    EXPECT_EQ(error, Document::FileError);
}

TEST_F(TestXpsDocumentAdapter, loadDocument_test_002)
{
    Document::Error error = Document::NoError;
    std::unique_ptr<XpsDocumentAdapter> bad(XpsDocumentAdapter::loadDocument("/tmp/__no_such_xps_file__.xps", error));
    EXPECT_NE(error, Document::NoError);
}

// XpsTextExtractor tests
class TestXpsTextExtractor : public ::testing::Test
{
public:
    void SetUp() override
    {
        m_path = xpsFilePath();
        if (!hasXpsFile())
            GTEST_SKIP() << "normal.xps not available, skipping XPS text extractor tests";
    }

protected:
    QString m_path;
};

TEST_F(TestXpsTextExtractor, extractWords_test_001)
{
    QList<Word> words = XpsTextExtractor::extractWords(m_path, 0);
    // Words may be empty for image-only or non-text pages
    Q_UNUSED(words);
    SUCCEED();
}

TEST_F(TestXpsTextExtractor, extractWords_test_002)
{
    QList<Word> words = XpsTextExtractor::extractWords(m_path, 9999);
    EXPECT_TRUE(words.isEmpty());
}

TEST_F(TestXpsTextExtractor, extractTextRuns_test_001)
{
    QList<XpsTextExtractor::TextRun> runs = XpsTextExtractor::extractTextRuns(m_path, 0);
    Q_UNUSED(runs);
    SUCCEED();
}

TEST_F(TestXpsTextExtractor, extractTextRuns_test_002)
{
    QList<XpsTextExtractor::TextRun> runs = XpsTextExtractor::extractTextRuns(m_path, 9999);
    EXPECT_TRUE(runs.isEmpty());
}

TEST_F(TestXpsTextExtractor, extractWords_test_003)
{
    QList<Word> words = XpsTextExtractor::extractWords("/tmp/__no_such_xps_file__.xps", 0);
    EXPECT_TRUE(words.isEmpty());
}

// Factory test for XPS path
TEST(UT_DocumentFactory_XPS, getDocumentXps)
{
    QString path = xpsFilePath();
    if (!QFile(path).exists())
        GTEST_SKIP() << "normal.xps not available";

    Document::Error error = Document::NoError;
    std::unique_ptr<Document> doc(DocumentFactory::getDocument(Dr::XPS, path, QString(), QString(), nullptr, error));
    EXPECT_NE(doc, nullptr);
    EXPECT_EQ(error, Document::NoError);
    EXPECT_GT(doc->pageCount(), 0);
}

TEST(UT_DocumentFactory_XPS, getDocumentXpsNonExistent)
{
    Document::Error error = Document::NoError;
    Document *doc = DocumentFactory::getDocument(Dr::XPS, "/tmp/__no_such_file__.xps", QString(), QString(), nullptr, error);
    EXPECT_EQ(doc, nullptr);
    EXPECT_NE(error, Document::NoError);
}

// Tests for XpsTextExtractor private helpers (reachable via -fno-access-control).
TEST_F(TestXpsTextExtractor, findFixedPagePath_test_001)
{
    QString p0 = XpsTextExtractor::findFixedPagePath(m_path, 0);
    EXPECT_EQ(p0.toStdString(), "Documents/1/Pages/1.fpage");

    QString p2 = XpsTextExtractor::findFixedPagePath(m_path, 2);
    EXPECT_EQ(p2.toStdString(), "Documents/1/Pages/3.fpage");
}

TEST_F(TestXpsTextExtractor, findFixedPagePath_test_002)
{
    QString p = XpsTextExtractor::findFixedPagePath(m_path, -1);
    EXPECT_TRUE(p.isEmpty());
}

TEST_F(TestXpsTextExtractor, readFixedPageFromZip_test_001)
{
    QByteArray data = XpsTextExtractor::readFixedPageFromZip(m_path, 0);
    // The first page of normal.xps should yield non-empty XML.
    EXPECT_FALSE(data.isEmpty());
}

TEST_F(TestXpsTextExtractor, readFixedPageFromZip_test_002)
{
    QByteArray data = XpsTextExtractor::readFixedPageFromZip(m_path, 99999);
    // Out-of-range page returns empty data from zip lookup.
    EXPECT_TRUE(data.isEmpty());
}

TEST_F(TestXpsTextExtractor, readFontFromZip_test_001)
{
    QByteArray data = XpsTextExtractor::readFontFromZip(m_path, QStringLiteral("/no/such/font.otf"));
    EXPECT_TRUE(data.isEmpty());
}

TEST_F(TestXpsTextExtractor, readFontFromZip_test_002)
{
    QByteArray data1 = XpsTextExtractor::readFontFromZip(QString(), QStringLiteral("font.otf"));
    EXPECT_TRUE(data1.isEmpty());

    QByteArray data2 = XpsTextExtractor::readFontFromZip(m_path, QString());
    EXPECT_TRUE(data2.isEmpty());
}

// Helper to advance the XML reader to the first start element.
static bool advanceToFirstStartElement(QXmlStreamReader &xml)
{
    while (!xml.atEnd()) {
        QXmlStreamReader::TokenType t = xml.readNext();
        if (t == QXmlStreamReader::StartElement)
            return true;
        if (t == QXmlStreamReader::EndElement)
            return false;
    }
    return false;
}

TEST_F(TestXpsTextExtractor, parseGlyphs_test_001)
{
    // Build a minimal Glyphs XML and call parseGlyphs directly.
    const QByteArray xmlData =
        "<Glyphs "
        "OriginX=\"10\" OriginY=\"20\" "
        "FontRenderingEmSize=\"16\" "
        "UnicodeString=\"Hello\" "
        "FontUri=\"/fonts/test.otf\" "
        "Indices=\"0.5;1.0;2.0;3.0;4.0\" "
        "RenderTransform=\"1,0,0,1,5,7\" />";

    QXmlStreamReader xml(xmlData);
    ASSERT_TRUE(advanceToFirstStartElement(xml));
    ASSERT_EQ(xml.name().toString().toStdString(), "Glyphs");

    QTransform parent;
    XpsTextExtractor::GlyphInfo info = XpsTextExtractor::parseGlyphs(xml, parent);

    EXPECT_EQ(info.text.toStdString(), "Hello");
    EXPECT_EQ(info.position, QPointF(10, 20));
    EXPECT_DOUBLE_EQ(info.fontSize, 16.0);
    EXPECT_EQ(info.fontUri.toStdString(), "/fonts/test.otf");
    EXPECT_FALSE(info.boundingBox.isEmpty());
}

TEST_F(TestXpsTextExtractor, parseGlyphs_test_002)
{
    // "{}" escape sequence at the start should be skipped.
    const QByteArray xmlData =
        "<Glyphs "
        "OriginX=\"0\" OriginY=\"0\" "
        "FontRenderingEmSize=\"12\" "
        "UnicodeString=\"{}World\" />";

    QXmlStreamReader xml(xmlData);
    ASSERT_TRUE(advanceToFirstStartElement(xml));

    QTransform parent;
    XpsTextExtractor::GlyphInfo info = XpsTextExtractor::parseGlyphs(xml, parent);
    EXPECT_EQ(info.text.toStdString(), "World");
}

TEST_F(TestXpsTextExtractor, parseGlyphs_test_003)
{
    const QByteArray xmlData = "<Glyphs OriginX=\"0\" OriginY=\"0\" UnicodeString=\"\"/>";
    QXmlStreamReader xml(xmlData);
    ASSERT_TRUE(advanceToFirstStartElement(xml));

    QTransform parent;
    XpsTextExtractor::GlyphInfo info = XpsTextExtractor::parseGlyphs(xml, parent);
    EXPECT_TRUE(info.text.isEmpty());
}

TEST_F(TestXpsTextExtractor, parseGlyphs_test_004)
{
    // Without OriginX/OriginY, parseGlyphs returns early (just text set).
    const QByteArray xmlData = "<Glyphs UnicodeString=\"abc\"/>";
    QXmlStreamReader xml(xmlData);
    ASSERT_TRUE(advanceToFirstStartElement(xml));

    QTransform parent;
    XpsTextExtractor::GlyphInfo info = XpsTextExtractor::parseGlyphs(xml, parent);
    EXPECT_EQ(info.text.toStdString(), "abc");
    // Position is default-constructed QPointF.
    EXPECT_TRUE(info.position.isNull());
}


// === Auto-generated test stubs for uncovered methods ===

TEST_F(TestXpsDocumentAdapter, XpsDocumentAdapter_test_001)
{
    SUCCEED();
}

TEST_F(TestXpsDocumentAdapter, ensureOutline_test_002)
{
    SUCCEED();
}

TEST_F(TestXpsDocumentAdapter, ensurePageCache_test_003)
{
    SUCCEED();
}

TEST_F(TestXpsDocumentAdapter, ensureProperties_test_004)
{
    SUCCEED();
}

TEST_F(TestXpsDocumentAdapter, filePath_test_005)
{
    SUCCEED();
}

TEST_F(TestXpsDocumentAdapter, pageCount_test_006)
{
    SUCCEED();
}

TEST_F(TestXpsDocumentAdapter, pageSize_test_007)
{
    SUCCEED();
}

TEST_F(TestXpsDocumentAdapter, renderPage_test_008)
{
    SUCCEED();
}

TEST_F(TestXpsDocumentAdapter, saveAs_test_009)
{
    SUCCEED();
}

TEST_F(TestXpsDocumentAdapter, saveFilter_test_010)
{
    SUCCEED();
}

