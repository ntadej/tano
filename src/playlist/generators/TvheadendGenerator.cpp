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

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QMessageBox>

#include "container/Channel.h"
#include "playlist/PlaylistModel.h"
#include "TvheadendGenerator.h"

TvheadendGenerator::TvheadendGenerator(const QString &location,
                                       const QString &interface,
                                       const QString &xmltv)
    : _location(location),
     _interface(interface),
     _xmltv(xmltv) { }

TvheadendGenerator::~TvheadendGenerator() { }

void TvheadendGenerator::clean()
{
    if(!QDir(_location + "/channels/").exists()) {
        QDir(_location).rmdir("channels");
    }

    if(!QDir(_location + "/channeltags/").exists()) {
        QDir(_location).rmdir("channeltags");
    }

    if(!QDir(_location + "/iptvservices/").exists()) {
        QDir(_location).rmdir("iptvservices");
    }

    if(!QDir(_location + "/iptvtransports/").exists()) {
        QDir(_location).rmdir("iptvtransports");
    }
}

bool TvheadendGenerator::write(PlaylistModel *model)
{
    clean();

    int id = 1;
    for (int i = 0; i < model->rowCount(); i++) {
        for(int c = 0; c < model->row(i)->categories().size(); c++) {
            if(!_tags.contains(model->row(i)->categories()[c])) {
                _tags.insert(model->row(i)->categories()[c], id);
                _tagsName.insert(id, model->row(i)->categories()[c]);
                id++;
            }
        }
    }

    for(int i = 1; i < id; i++) {
        generateTag(i, _tagsName[i]);
    }

    for (int i = 0; i < model->rowCount(); i++) {
        generateItem(model->row(i));
	}
	return true;
}

QString TvheadendGenerator::fileChannel(const int &number) const
{
    if(!QDir(_location + "/channels/").exists()) {
        QDir(_location).mkdir("channels");
    }
    return QString(_location + "/channels/" + QString::number(number));
}

QString TvheadendGenerator::fileIpService(const int &number) const
{
    if(!QDir(_location + "/iptvservices/").exists()) {
        QDir(_location).mkdir("iptvservices");
    }
    return QString(_location + "/iptvservices/" + "iptv_" + QString::number(number));
}

QString TvheadendGenerator::fileIpTransport(const int &number) const
{
    if(!QDir(_location + "/iptvtransports/").exists()) {
        QDir(_location).mkdir("iptvtransports");
    }
    return QString(_location + "/iptvtransports/" + "iptv_" + QString::number(number));
}

QString TvheadendGenerator::fileTag(const int &number) const
{
    if(!QDir(_location + "/channeltags/").exists()) {
        QDir(_location).mkdir("channeltags");
    }
    return QString(_location + "/channeltags/" + QString::number(number));
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
         << indent(1) << "\"xmltv-channel\": \"" << channel->epg() << _xmltv << "\"," << "\n"
         << indent(1) << "\"icon\": \"" << channel->logo() << "\"," << "\n"
         << indent(1) << "\"tags\": [" << "\n";
    for(int i = 0; i < channel->categories().size(); i++) {
        outC << indent(2) << _tags[channel->categories()[i]];
        if(i != channel->categories().size()-1)
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

    QTextStream outIS(&fIpService);
    outIS << "{" << "\n"
          << indent(1) << "\"pmt\": 0," << "\n"
          << indent(1) << "\"port\": " << channel->url().replace(QRegExp("udp://@.*:"), "") << "," << "\n"
          << indent(1) << "\"interface\": \"" << _interface << "\"," << "\n"
          << indent(1) << "\"group\": \"" << channel->url().replace(QRegExp("udp://@"), "").replace(QRegExp(":.*"), "") << "\"," << "\n"
          << indent(1) << "\"channelname\": \"" << channel->name() << "\"," << "\n"
          << indent(1) << "\"mapped\": 1," << "\n"
          << indent(1) << "\"pcr\": 0," << "\n"
          << indent(1) << "\"disabled\": 0" << "\n"
          << "}" << "\n";


    QFile fIpTransport(fileIpTransport(channel->number()));
    if (!fIpTransport.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("Tano"),
                            QObject::tr("Cannot write file %1:\n%2.")
                            .arg(fileIpTransport(channel->number()))
                            .arg(fIpTransport.errorString()));
        return;
    }

    QTextStream outIT(&fIpTransport);
    outIT << "{" << "\n"
          << indent(1) << "\"pmt\": 0," << "\n"
          << indent(1) << "\"port\": " << channel->url().replace(QRegExp("udp://@.*:"), "") << "," << "\n"
          << indent(1) << "\"interface\": \"" << _interface << "\"," << "\n"
          << indent(1) << "\"group\": \"" << channel->url().replace(QRegExp("udp://@"), "").replace(QRegExp(":.*"), "") << "\"," << "\n"
          << indent(1) << "\"channelname\": \"" << channel->name() << "\"," << "\n"
          << indent(1) << "\"mapped\": 1," << "\n"
          << indent(1) << "\"pcr\": 0," << "\n"
          << indent(1) << "\"disabled\": 0" << "\n"
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
