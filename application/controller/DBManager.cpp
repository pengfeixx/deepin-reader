/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
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
#include "DBManager.h"

#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QStandardPaths>

#include "application.h"

#include "subjectObserver/MsgHeader.h"
#include "utils/utils.h"

void DBManager::getBookMarks()
{
    m_pBookMarkList.clear();
    m_nDbType = -1;
    const QSqlDatabase db = getDatabase();
    if (db.isValid()) {
        QMutexLocker mutex(&m_mutex);
        QSqlQuery query(db);
        query.setForwardOnly(true);
        query.prepare("SELECT PageNumber FROM BookMarkTable where FilePath = ? and FileName = ? ORDER BY Time desc");
        query.addBindValue(m_strFilePath);
        query.addBindValue(m_strFileName);
        if (query.exec()) {
            QString sData = "";

            while (query.next()) {
                m_nDbType = 0;
                sData = query.value(0).toString();      //  结果
            }

            QStringList sPageList = sData.split(",", QString::SkipEmptyParts);
            foreach (QString s, sPageList) {
                int nPage = s.toInt();
                m_pBookMarkList.append(nPage);
            }

            qSort(m_pBookMarkList.begin(), m_pBookMarkList.end());
        }
    }
}

//  保存数据
void DBManager::saveBookMark()
{
    if (m_pBookMarkList.size() == 0) {
        deleteBookMark();
    } else {
        if (m_nDbType == -1) {    //  原来没有数据
            QString sPage = "";
            foreach (int i, m_pBookMarkList) {
                sPage += QString::number(i) + ",";
            }
            insertBookMark(sPage);
        } else {
            QString sPage = "";
            foreach (int i, m_pBookMarkList) {
                sPage += QString::number(i) + ",";
            }

            updateBookMark(sPage);
        }
    }
}

//  新增标签数据
void DBManager::insertBookMark(const QString &pageNumber, const QString &strFilePath, const QString &strFileName)
{
    const QSqlDatabase db = getDatabase();

    QMutexLocker mutex(&m_mutex);
    // Insert into BookMarkTable
    QSqlQuery query(db);
    query.setForwardOnly(true);
    query.exec("START TRANSACTION");//开始事务。使用BEGIN也可以
    query.prepare("INSERT INTO BookMarkTable "
                  "(FilePath, FileName, PageNumber, Time) VALUES (?, ?, ?, ?)");
    if (strFilePath != "") {
        query.addBindValue(strFilePath);
    } else {
        query.addBindValue(m_strFilePath);
    }

    if (strFileName != "") {
        query.addBindValue(strFileName);
    } else {
        query.addBindValue(m_strFileName);
    }

    query.addBindValue(pageNumber);

    QString nowTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.addBindValue(nowTime);

    if (query.exec()) {
        query.exec("COMMIT");
    } else {
        query.exec("ROLLBACK");//回滚
    }
}

//  更新标签数据
void DBManager::updateBookMark(const QString &pageNumber)
{
    const QSqlDatabase db = getDatabase();

    QMutexLocker mutex(&m_mutex);
    // update BookMarkTable
    QSqlQuery query(db);
    query.setForwardOnly(true);
    query.exec("START TRANSACTION");//开始事务。使用BEGIN也可以
    query.prepare("UPDATE BookMarkTable "
                  "set PageNumber = ? where FilePath = ? and FileName = ?");
    query.addBindValue(pageNumber);
    query.addBindValue(m_strFilePath);
    query.addBindValue(m_strFileName);

    if (query.exec()) {
        query.exec("COMMIT");
    }
}

void DBManager::deleteBookMark()
{
    const QSqlDatabase db = getDatabase();

    QMutexLocker mutex(&m_mutex);
    // delete into BookMarkTable
    QSqlQuery query(db);
    query.setForwardOnly(true);
    query.exec("START TRANSACTION");//开始事务。使用BEGIN也可以
    query.prepare("DELETE FROM BookMarkTable "
                  "where FilePath = ? and FileName = ?");
    query.addBindValue(m_strFilePath);
    query.addBindValue(m_strFileName);

    if (query.exec()) {
        query.exec("COMMIT");
    }
}

/**
 * @brief DBManager::clearInvalidRecord
 * 删除不存在文件(绝对路径)
 */
void DBManager::clearInvalidRecord()
{
    const QSqlDatabase db = getDatabase();
    QMutexLocker mutex(&m_mutex);
    QSqlQuery query(db);
    query.prepare("select FilePath from BookMarkTable");
    if (query.exec()) {
        QString strsql;
        while (query.next()) {
            QString strpath = query.value(0).toString();
            if (!QFile::exists(strpath)) {
                strsql.append(QString("delete from BookMarkTable where FilePath='%1';").arg(strpath));
            }
        }
        query.clear();
        if (!strsql.isEmpty()) {
            query.prepare(strsql);
            if (!query.exec())
                qDebug() << __LINE__ << "   " << __FUNCTION__ << "   " << query.lastError();
        }
    }
}

bool DBManager::hasFilePathDB(const QString &filePath)
{
    const QSqlDatabase db = getDatabase();
    QMutexLocker mutex(&m_mutex);
    QSqlQuery query(db);
    QString sql = QString("select FilePath from FilesTable where FilePath = '%1'").arg(filePath);
//    query.prepare("select FilePath from FileFontTable where FilePath = ?");
//    query.addBindValue(filePath);
    if (query.exec(sql)) {
        while (query.next()) {
            qDebug() << "  FilePath:" << query.value(0).toString();
            if (query.value(0).toString() != "")
                return true;
        }
    }
    return false;
}

DBManager::DBManager(QObject *parent)
    : QObject(parent)
    , m_connectionName("default_connection")
{
    checkDatabase();
    clearInvalidRecord();
}

DBManager::~DBManager()
{

}


const QSqlDatabase DBManager::getDatabase()
{
    QMutexLocker mutex(&m_mutex);
    if (QSqlDatabase::contains(m_connectionName)) {
        QSqlDatabase db = QSqlDatabase::database(m_connectionName);
        return db;
    } else {
        //if database not open, open it.
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);//not dbConnection
        QString sDbPath = Utils::getConfigPath();
        db.setDatabaseName(sDbPath + "/" + ConstantMsg::g_db_name);
        if (! db.open()) {
            qWarning() << "Open database error:" << db.lastError();
            return QSqlDatabase();
        } else {
            return db;
        }
    }
}

void DBManager::checkDatabase()
{
    const QSqlDatabase db = getDatabase();
    if (! db.isValid()) {
        return;
    }
    bool tableExist = false;
    QMutexLocker mutex(&m_mutex);
    QSqlQuery query(db);
    query.setForwardOnly(true);
    query.prepare("SELECT name FROM sqlite_master "
                  "WHERE type=\"table\" AND name = \"BookMarkTable\"");
    if (query.exec() && query.first()) {
        tableExist = !query.value(0).toString().isEmpty();
    }

    //if tables not exist, create it.
    if (!tableExist) {
        QSqlQuery query(db);
        // ImageTable3
        //////////////////////////////////////////////////////////////
        //PathHash           | FilePath | FileName   | Dir  | Time  //
        //TEXT primari key   | TEXT     | TEXT       | TEXT | TEXT  //
        //////////////////////////////////////////////////////////////
        query.exec(QString("CREATE TABLE IF NOT EXISTS BookMarkTable ( "
                           "FilePath TEXT primary key, "
                           "FileName TEXT, "
                           "PageNumber TEXT, "
                           "Time TEXT )"));
    }
}

void DBManager::setStrFilePath(const QString &strFilePath)
{
    m_strFilePath = strFilePath;

    int nLastPos = strFilePath.lastIndexOf('/');
    nLastPos++;
    m_strFileName = strFilePath.mid(nLastPos);
}

bool  DBManager::saveasBookMark(const QString &oldpath, const QString &newpath)
{
    bool bsuccess = false;
    const QSqlDatabase db = getDatabase();
    QMutexLocker mutex(&m_mutex);
    QSqlQuery query(db);
    query.setForwardOnly(true);
    query.prepare("select count(*) from BookMarkTable where FilePath=?");
    query.addBindValue(newpath);
    if (query.exec()) {
        QString strnum;
        if (query.next() && query.value(0).toInt() <= 0 && m_pBookMarkList.count() > 0) {
            foreach (int i, m_pBookMarkList) {
                strnum += QString::number(i) + ",";
            }
            QString strfilename = newpath.mid(newpath.lastIndexOf("/") + 1);
            query.clear();
            query.exec("START TRANSACTION");

            query.prepare("INSERT INTO BookMarkTable "
                          "(FilePath, FileName, PageNumber, Time) VALUES (?, ?, ?, ?)");
            QString nowTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

            query.addBindValue(newpath);
            query.addBindValue(strfilename);
            query.addBindValue(strnum);
            query.addBindValue(nowTime);

            if (query.exec()) {
                query.exec("COMMIT");
                bsuccess = true;
            }
        }
    }
    return  bsuccess;
}

void DBManager::setBookMarkList(const QList<int> &pBookMarkList)
{
    m_pBookMarkList = pBookMarkList;
}

QList<int> DBManager::getBookMarkList() const
{
    return m_pBookMarkList;
}
