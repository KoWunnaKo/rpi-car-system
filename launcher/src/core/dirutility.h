/*
 * This file is part of RPI Car System.
 * Copyright (c) 2016 Fabien Caylus <tranqyll.dev@gmail.com>
 *
 * This file is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DIRUTILITY_H
#define DIRUTILITY_H

#include <random>

#include <QString>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QDirIterator>
#include <QFileInfo>
#include <QStorageInfo>
#include <QDateTime>
#include <QByteArray>
#include <QCryptographicHash>

#include "common.h"

namespace DirUtility
{
    static inline QString readFile(const QString &fileName)
    {
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text))
            return QString();
        QTextStream in(&file);
        return in.readAll();
    }

    static inline void writeFile(const QString& fileName, const QString& content)
    {
        QFile file(fileName);
        if(file.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream out(&file);
            out << content;
        }
    }

    static inline qint64 dirSize(const QString& dirPath)
    {
        qint64 size = 0;
        QDirIterator it(dirPath,
                        QDir::Dirs | QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable | QDir::Hidden,
                        QDirIterator::Subdirectories);
        while(it.hasNext())
        {
            it.next();
            size += it.fileInfo().size();
        }

        return size;
    }

    static inline bool removeRecursively(const QString &dirName)
    {
        bool result = true;
        QDir dir(dirName);

        if(dir.exists(dirName))
        {
            QFileInfoList entriesList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
            for(QFileInfo info : entriesList)
            {
                if(info.isDir())
                    result = removeRecursively(info.absoluteFilePath());
                else
                    result = QFile::remove(info.absoluteFilePath());

                if(!result)
                    return result;
            }

            result = dir.rmdir(dirName);
        }

        return result;
    }

    static inline bool copyRecursively(const QString &srcFilePath, const QString &tgtFilePath)
    {
        QFileInfo srcFileInfo(srcFilePath);

        if(srcFileInfo.isDir())
        {
            QDir targetDir(tgtFilePath);
            targetDir.cdUp();

            if(!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
                return false;

            QDir sourceDir(srcFilePath);
            QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

            for(const QString &fileName : fileNames)
            {
                const QString newSrcFilePath = srcFilePath + QLatin1Char('/') + fileName;
                const QString newTgtFilePath = tgtFilePath + QLatin1Char('/') + fileName;
                if(!copyRecursively(newSrcFilePath, newTgtFilePath))
                    return false;
            }
        }
        else
        {
            if(!QFile::copy(srcFilePath, tgtFilePath))
                return false;
        }

        return true;
    }

    static inline void listUSBDevices(QStringList *pathList, QStringList *nameList = nullptr)
    {
        if(pathList == nullptr)
            return;

        pathList->clear();
        if(nameList)
            nameList->clear();

        for(QStorageInfo info: QStorageInfo::mountedVolumes())
        {
            if(info.isReady() && info.isValid() && !info.isRoot() && info.rootPath().startsWith("/media"))
            {
                pathList->append(info.rootPath());
                if(nameList)
                {
                    if(info.name().isEmpty())
                        nameList->append(QCoreApplication::tr("%1 device").arg(Common::bytesSizeToString(info.bytesTotal())));
                    else
                    {
                        QString name = info.name();
                        // This code handle \xHH characters (for example \x20 for a space)
                        while(name.contains("\\x"))
                        {
                            int index = name.indexOf("\\x");
                            char ch = name.mid(index + 2, 2).toInt(nullptr, 16);
                            name.replace(name.indexOf("\\x"), 4, QChar(ch));
                        }
                        nameList->append(name + " (" + Common::bytesSizeToString(info.bytesTotal()) + ")");
                    }
                }
            }
        }
    }

    // Check if the file content equals to <content>
    static inline bool checkContent(const QString& fileName, QByteArray& content)
    {
        QCryptographicHash hash1(QCryptographicHash::Sha1);
        QFile file(fileName);
        if(file.open(QFile::ReadOnly))
        {
            hash1.addData(file.readAll());
            file.close();

            QCryptographicHash hash2(QCryptographicHash::Sha1);
            hash2.addData(content);

            return hash1.result() == hash2.result();
        }
        return false;
    }

    // Get a unique filename
    // Use the date-time and a random number
    // /!\ It's not guaranted to be unique
    static inline QString uniqueFileName()
    {
        // Generate random number
        std::minstd_rand generator;
        std::uniform_int_distribution<> distrib(0, 10000000);
        const int randNb = distrib(generator);

        return QDateTime::currentDateTime().toString("ddMMyyyy-HHmmsszzz") + QLatin1String("-") + QString::number(randNb);
    }
}

#endif // DIRUTILITY_H

