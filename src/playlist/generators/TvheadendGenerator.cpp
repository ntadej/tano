/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#if defined(Qt5)
    #include <QtWidgets/QMessageBox>
#elif defined(Qt4)
    #include <QtGui/QMessageBox>
#endif

#include "container/core/Channel.h"
#include "playlist/PlaylistModel.h"
#include "TvheadendGenerator.h"

TvheadendGenerator::TvheadendGenerator(const QString &location,
                                       const QString &interface)
    : _location(location),
     _interface(interface) { }

TvheadendGenerator::~TvheadendGenerator() { }

void TvheadendGenerator::clean()
{
    if (!QDir(_location + "/channels/").exists()) {
        QDir(_location).rmdir("channels");
    }

    if (!QDir(_location + "/channeltags/").exists()) {
        QDir(_location).rmdir("channeltags");
    }

    if (!QDir(_location + "/iptvservices/").exists()) {
        QDir(_location).rmdir("iptvservices");
    }

    if (!QDir(_location + "/epggrab/xmltv/channels/").exists()) {
        QDir(_location + "/epggrab/xmltv/").rmdir("channels");
    }
}

bool TvheadendGenerator::write(PlaylistModel *model)
{
    clean();

    int id = 1;
    for (int i = 0; i < model->rowCount(); i++) {
        foreach (const QString &category, model->row(i)->categories()) {
            if (!_tags.contains(category)) {
                _tags.insert(category, id);
                _tagsName.insert(id, category);
                id++;
            }
        }
    }

    for (int i = 1; i < id; i++) {
        generateTag(i, _tagsName[i]);
    }

    for (int i = 0; i < model->rowCount(); i++) {
        generateItem(model->row(i));
	}
	return true;
}

QString TvheadendGenerator::fileChannel(const int &number) const
{
    if (!QDir(_location + "/channels/").exists()) {
        QDir(_location).mkdir("channels");
    }
    return QString(_location + "/channels/" + QString::number(number));
}

QString TvheadendGenerator::fileIpService(const int &number) const
{
    if (!QDir(_location + "/iptvservices/").exists()) {
        QDir(_location).mkdir("iptvservices");
    }
    return QString(_location + "/iptvservices/" + "iptv_" + QString::number(number));
}

QString TvheadendGenerator::fileTag(const int &number) const
{
    if (!QDir(_location + "/channeltags/").exists()) {
        QDir(_location).mkdir("channeltags");
    }
    return QString(_location + "/channeltags/" + QString::number(number));
}

QString TvheadendGenerator::fileXmltv(const QString &name) const
{
    if (!QDir(_location + "/epggrab/xmltv/channels/").exists()) {
        QDir(_location).mkdir("epggrab");
        QDir(_location + "/epggrab").mkdir("xmltv");
        QDir(_location + "/epggrab/xmltv").mkdir("channels");
    }
    return QString(_location + "/epggrab/xmltv/channels/" + name);
}

void TvheadendGenerator::generateItem(Channel *channel)
{
    QFile fChannel(fileChannel(channel->number()));
    if (!fChannel.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("Tano"),
                            QObject::tr("Cannot write file %1:\n%2.")
                            .arg(fileChannel(channel->number()))
                            .arg(fChannel.errorString()));
        return;
    }

    QTextStream outC(&fChannel);
    outC << "{" << "\n"
         << indent(1) << "\"name\": \"" << channel->name() << "\"," << "\n"
         << indent(1) << "\"icon\": \"" << channel->logo() << "\"," << "\n"
         << indent(1) << "\"tags\": [" << "\n";
    foreach (const QString &category, channel->categories()) {
        outC << indent(2) << _tags[category];
        if (category != channel->categories().last())
            outC << ",";
        outC << "\n";
    }
    outC << indent(1) << "]," << "\n"
         << indent(1) << "\"dvr_extra_time_pre\": 0," << "\n"
         << indent(1) << "\"dvr_extra_time_post\": 0," << "\n"
         << indent(1) << "\"channel_number\": " << channel->number() << "\n"
         << "}" << "\n";


    QFile fIpService(fileIpService(channel->number()));
    if (!fIpService.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("Tano"),
                            QObject::tr("Cannot write file %1:\n%2.")
                            .arg(fileIpService(channel->number()))
                            .arg(fIpService.errorString()));
        return;
    }

    int type = 1;
    switch (channel->type())
    {
    case Channel::Radio:
        type = 2;
        break;
    case Channel::HD:
        type = 17;
        break;
    case Channel::SD:
    default:
        type = 1;
        break;
    }

    QTextStream outIS(&fIpService);
    outIS << "{" << "\n"
          << indent(1) << "\"pmt\": 0," << "\n"
          << indent(1) << "\"stype\": " << type << "," << "\n"
          << indent(1) << "\"port\": " << channel->url().replace(QRegExp("udp://@.*:"), "") << "," << "\n"
          << indent(1) << "\"interface\": \"" << _interface << "\"," << "\n"
          << indent(1) << "\"group\": \"" << channel->url().replace(QRegExp("udp://@"), "").replace(QRegExp(":.*"), "") << "\"," << "\n"
          << indent(1) << "\"channelname\": \"" << channel->name() << "\"," << "\n"
          << indent(1) << "\"mapped\": 1," << "\n"
          << indent(1) << "\"pcr\": 0," << "\n"
          << indent(1) << "\"disabled\": 0" << "\n"
          << "}" << "\n";

    if (channel->xmltvId().isEmpty())
        return;

    QFile fXmltv(fileXmltv(channel->xmltvId()));
    if (!fXmltv.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << QObject::tr("Error:") << QObject::tr("Cannot write file %1:\n%2.")
                                                         .arg(fileXmltv(channel->xmltvId()))
                                                         .arg(fXmltv.errorString());
        return;
    }

    QTextStream outX(&fXmltv);
    outX << "{" << "\n"
         << indent(1) << "\"name\": \"" << channel->name() << "\"," << "\n"
         << indent(1) << "\"channels\": [" << "\n"
         << indent(2) << channel->number() << "\n"
         << indent(1) << "]" << "\n"
         << "}" << "\n";
}

void TvheadendGenerator::generateTag(const int &id,
                                     const QString &name)
{
    QFile fTag(fileTag(id));
    if (!fTag.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("Tano"),
                            QObject::tr("Cannot write file %1:\n%2.")
                            .arg(fileTag(id))
                            .arg(fTag.errorString()));
        return;
    }

    QTextStream outF(&fTag);
    outF << "{" << "\n"
         << indent(1) << "\"enabled\": 1," << "\n"
         << indent(1) << "\"internal\": 0," << "\n"
         << indent(1) << "\"titledIcon\": 0," << "\n"
         << indent(1) << "\"name\": \"" << name << "\"," << "\n"
         << indent(1) << "\"comment\": \"\"," << "\n"
         << indent(1) << "\"icon\": \"\"," << "\n"
         << indent(1) << "\"id\": " << id << "\n"
         << "}" << "\n";
}

QString TvheadendGenerator::indent(const int &indentLevel) const
{
    return QString(indentLevel, '\t');
}

int TvheadendGenerator::tag(const QString &name) const
{
    return _tags[name];
}
