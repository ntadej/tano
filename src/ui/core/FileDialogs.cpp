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

QString FileDialogs::filterByType(const Tano::FileType &type)
{
    switch (type)
    {
    case Tano::Files:
        return QObject::tr("Multimedia files") + "(*)";
    case Tano::Subtitles:
        return QObject::tr("Subtitles files") + "(*.sub *.srt *.txt)";
    case Tano::M3U:
        return QObject::tr("Tano M3U channel list files") + "(*.m3u)";
    case Tano::M3UClean:
        return QObject::tr("M3U (original) files") + "(*.m3u tano11461)";
    case Tano::M3UUdpxy:
        return QObject::tr("M3U (Udpxy URL) files") + "(*.m3u tano63848)";
    case Tano::CSV:
        return QObject::tr("Comma-separated values files") + "(*.csv *.txt)";
    case Tano::JS:
        return QObject::tr("Sagem STB channel list files") + "(*.js)";
    case Tano::XmltvId:
        return QObject::tr("Plain text files") + "(*.txt)";
    case Tano::TanoOld:
        return QObject::tr("Tano TV old channel list files") + "(*.tano *.xml)";
    default:
        return QString();
    }
}

QString FileDialogs::openByType(const Tano::FileType &type,
                                const QString &arg)
{
    switch (type)
    {
    case Tano::Directory:
        return openDirectory(arg);
    case Tano::M3U:
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
                                                QDir::homePath(), filterByType(Tano::Files));
    return file;
}

File FileDialogs::openPlaylist()
{
    QStringList filters;
    filters << filterByType(Tano::M3U)
            << filterByType(Tano::CSV)
            << filterByType(Tano::JS)
            << filterByType(Tano::TanoOld);

    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilters(filters);

    QString fileName;
    int type = -1;
    if(dialog.exec()) {
        fileName = dialog.selectedFiles()[0];

        for(int i = 0; i < 50; i++) {
            if(dialog.selectedNameFilter() == filterByType(Tano::FileType(i))) {
                type = i;
            }
        }
    }

    return File(fileName, type);
}

QString FileDialogs::openPlaylistSimple()
{
    QString file = QFileDialog::getOpenFileName(0, QObject::tr("Open channel list"),
                                                QDir::homePath(), filterByType(Tano::M3U));
    return file;
}

QString FileDialogs::openSubtitles(const QString &dir)
{
    QString file = QFileDialog::getOpenFileName(0, QObject::tr("Open subtitles"),
                                                dir, filterByType(Tano::Subtitles));
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
    filters << filterByType(Tano::M3U)
            << filterByType(Tano::M3UClean)
            << filterByType(Tano::M3UUdpxy)
            << filterByType(Tano::CSV)
            << filterByType(Tano::JS);

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
            if(dialog.selectedNameFilter() == filterByType(Tano::FileType(i))) {
                type = i;
            }
        }
    }

    return File(fileName, type);
}

QString FileDialogs::saveXmltv()
{
    QString file =  QFileDialog::getSaveFileName(0, QObject::tr("Export XMLTV IDs"),
                                                 QDir::homePath(), filterByType(Tano::XmltvId));
    return file;
}
