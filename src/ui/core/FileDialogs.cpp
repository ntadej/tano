/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QObject>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>

#include "container/File.h"
#include "FileDialogs.h"

QString FileDialogs::filterByType(const Type &type)
{
    switch (type)
    {
    case Files:
        return QObject::tr("Multimedia files") + "(*)";
    case Subtitles:
        return QObject::tr("Subtitles files") + "(*.sub *.srt *.txt)";
    case M3U:
        return QObject::tr("Tano M3U channel list files") + "(*.m3u)";
    case M3UClean:
        return QObject::tr("M3U (original) files") + "(*.m3u tano11461)";
    case M3UUdpxy:
        return QObject::tr("M3U (Udpxy URL) files") + "(*.m3u tano63848)";
    case CSV:
        return QObject::tr("Comma-separated values files") + "(*.csv *.txt)";
    case JS:
        return QObject::tr("Sagem STB channel list files") + "(*.js)";
    case Xmltv:
        return QObject::tr("Plain text files") + "(*.txt)";
    case TanoOld:
        return QObject::tr("Tano TV old channel list files") + "(*.tano *.xml)";
    default:
        return QString();
    }
}

QString FileDialogs::openByType(const Type &type,
                                const QString &arg)
{
    switch (type)
    {
    case Directory:
        return openDirectory(arg);
    case M3U:
        return openPlaylistSimple();
    default:
        return QString();
    }
}

QString FileDialogs::openDirectory(const QString &dir)
{
    QString file = QFileDialog::getExistingDirectory(0, QObject::tr("Open directory"),
                                                    dir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    return file;
}

QString FileDialogs::openFile()
{
    QString file = QFileDialog::getOpenFileName(0, QObject::tr("Open file"),
                                                QDir::homePath(), filterByType(Files));
    return file;
}

File FileDialogs::openPlaylist()
{
    QStringList filters;
    filters << filterByType(M3U)
            << filterByType(CSV)
            << filterByType(JS)
            << filterByType(TanoOld);

    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilters(filters);

    QString fileName;
    int type = -1;
    if(dialog.exec()) {
        fileName = dialog.selectedFiles()[0];

        for(int i = 0; i < 50; i++) {
            if(dialog.selectedNameFilter() == filterByType(Type(i))) {
                type = i;
            }
        }
    }

    return File(fileName, type);
}

QString FileDialogs::openPlaylistSimple()
{
    QString file = QFileDialog::getOpenFileName(0, QObject::tr("Open channel list"),
                                                QDir::homePath(), filterByType(M3U));
    return file;
}

QString FileDialogs::openSubtitles(const QString &dir)
{
    QString file = QFileDialog::getOpenFileName(0, QObject::tr("Open subtitles"),
                                                dir, filterByType(Subtitles));
    return file;
}

QString FileDialogs::openUrl()
{
    bool ok;
    QString file = QInputDialog::getText(0, QObject::tr("Open URL or stream"),
                                         QObject::tr("Enter the URL of multimedia file or stream you want to play:"),
                                         QLineEdit::Normal, "", &ok);
    if(!ok)
        return QString("");
    else
        return file;
}

File FileDialogs::savePlaylist()
{
    QStringList filters;
    filters << filterByType(M3U)
            << filterByType(M3UClean)
            << filterByType(M3UUdpxy)
            << filterByType(CSV)
            << filterByType(JS);

    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setConfirmOverwrite(true);
    dialog.setNameFilters(filters);

    QString fileName;
    int type = -1;
    if(dialog.exec()) {
        fileName = dialog.selectedFiles()[0];

        for(int i = 0; i < 50; i++) {
            if(dialog.selectedNameFilter() == filterByType(Type(i))) {
                type = i;
            }
        }
    }

    return File(fileName, type);
}

QString FileDialogs::saveXmltv()
{
    QString file =  QFileDialog::getSaveFileName(0, QObject::tr("Export XMLTV IDs"),
                                                 QDir::homePath(), filterByType(Xmltv));
    return file;
}
