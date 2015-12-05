/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>

#include "FileDialogs.h"

QString FileDialogs::filterByType(const File::Type &type)
{
    switch (type)
    {
    case File::Files:
        return QObject::tr("Multimedia files") + "(*)";
    case File::Subtitles:
        return QObject::tr("Subtitles files") + "(*.sub *.srt *.txt)";
    case File::M3U:
        return QObject::tr("Tano M3U channel list files") + "(*.m3u)";
    case File::M3UClean:
        return QObject::tr("M3U (original) files") + "(*.m3u)";
    case File::M3UUdpxy:
        return QObject::tr("M3U (Udpxy URL) files") + "(*.m3u)";
    case File::CSV:
        return QObject::tr("Comma-separated values files") + "(*.csv *.txt)";
    case File::XmltvFile:
        return QObject::tr("XMLTV file") + "(*.xml)";
    case File::XmltvId:
        return QObject::tr("Plain text files") + "(*.txt)";
    default:
        return QString();
    }
}

QString FileDialogs::openByType(const File::Type &type,
                                const QString &arg)
{
    switch (type)
    {
    case File::Directory:
        return openDirectory(arg);
    case File::M3U:
        return openChannelsSimple(arg);
    case File::XmltvFile:
        return openXmltvFile(arg);
    default:
        return QString();
    }
}

QString FileDialogs::openDirectory(const QString &dir)
{
    return QFileDialog::getExistingDirectory(0, QObject::tr("Open directory"),
                                             dir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

QString FileDialogs::openFile(const QString &file)
{
    return QFileDialog::getOpenFileName(0, QObject::tr("Open file"),
                                        file, filterByType(File::Files));
}

File FileDialogs::openChannelsList()
{
    QStringList filters;
    filters << filterByType(File::M3U)
            << filterByType(File::CSV);

    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilters(filters);

    QString fileName;
    File::Type type = File::Unknown;
    if (dialog.exec()) {
        fileName = dialog.selectedFiles()[0];

        for (int i = 0; i < 50; i++) {
            if (dialog.selectedNameFilter() == filterByType(File::Type(i))) {
                type = File::Type(i);
            }
        }
    }

    File file;
    file.path = fileName;
    file.type = type;
    return file;
}

QString FileDialogs::openChannelsSimple(const QString &file)
{
    return QFileDialog::getOpenFileName(0, QObject::tr("Open channel list"),
                                        file, filterByType(File::M3U));
}

QString FileDialogs::openSubtitles(const QString &file)
{
    return QFileDialog::getOpenFileName(0, QObject::tr("Open subtitles"),
                                        file, filterByType(File::Subtitles));
}

QString FileDialogs::openUrl()
{
    bool ok;
    QString file = QInputDialog::getText(0, QObject::tr("Open URL or stream"),
                                         QObject::tr("Enter the URL of multimedia file or stream you want to play:"),
                                         QLineEdit::Normal, "", &ok);
    return ok ? file : QString();
}

QString FileDialogs::openXmltvFile(const QString &file)
{
    return QFileDialog::getOpenFileName(0, QObject::tr("Open XMLTV"),
                                        file, filterByType(File::XmltvFile));
}

File FileDialogs::saveChannelsList()
{
    QStringList filters;
    filters << filterByType(File::M3U)
            << filterByType(File::M3UClean)
            << filterByType(File::M3UUdpxy)
            << filterByType(File::CSV);

    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setConfirmOverwrite(true);
    dialog.setNameFilters(filters);

    QString fileName;
    File::Type type = File::Unknown;
    if (dialog.exec()) {
        fileName = dialog.selectedFiles()[0];

        for (int i = 0; i < 50; i++) {
            if (dialog.selectedNameFilter() == filterByType(File::Type(i))) {
                type = File::Type(i);
            }
        }
    }

    File file;
    file.path = fileName;
    file.type = type;
    return file;
}

QString FileDialogs::saveXmltv(const QString &file)
{
    return QFileDialog::getSaveFileName(0, QObject::tr("Export XMLTV IDs"),
                                        file, filterByType(File::XmltvId));
}
