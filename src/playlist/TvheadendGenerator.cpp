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
#include "TvheadendGenerator.h"

TvheadendGenerator::TvheadendGenerator(QTreeWidget *treeWidget,
									   QMap<QTreeWidgetItem *, Channel *> map,
									   const QString &location,
									   const QString &interface,
									   const QString &xmltv)
	: _treeWidget(treeWidget),
	_map(map),
	_location(location),
	_interface(interface),
	_xmltv(xmltv) { }

TvheadendGenerator::~TvheadendGenerator() { }

bool TvheadendGenerator::write()
{
    if(!QDir(_location + "/channels/").exists()) {
        QDir(_location).rmdir("channels");
    }

    if(!QDir(_location + "/iptvservices/").exists()) {
        QDir(_location).rmdir("iptvservices");
    }

	for (int i = 0; i < _treeWidget->topLevelItemCount(); ++i) {
		generateItem(_map[_treeWidget->topLevelItem(i)]);
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

void TvheadendGenerator::generateItem(Channel *channel)
{
    QFile fChannel(fileChannel(channel->number()));
    if (!fChannel.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(_treeWidget, QObject::tr("Tano"),
                            QObject::tr("Cannot write file %1:\n%2.")
                            .arg(fileChannel(channel->number()))
                            .arg(fChannel.errorString()));
        return;
    }

    QTextStream outC(&fChannel);
    outC << "{" << "\n"
         << indent(1) << "\"name\": \"" << channel->name() << "\"," << "\n"
         << indent(1) << "\"xmltv-channel\": \"" << channel->epg() << _xmltv << "\"," << "\n"
         << indent(1) << "\"tags\": [" << "\n"
         << indent(1) << "]," << "\n"
         << indent(1) << "\"dvr_extra_time_pre\": 0," << "\n"
         << indent(1) << "\"dvr_extra_time_post\": 0," << "\n"
         << indent(1) << "\"channel_number\": " << channel->number() << "," << "\n"
         << "}" << "\n";

    fChannel.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner |
                            QFile::ReadUser | QFile::WriteUser | QFile::ExeUser |
                            QFile::ReadGroup | QFile::WriteGroup | QFile::ExeGroup |
                            QFile::ReadOther | QFile::WriteOther | QFile::ExeOther);


    QFile fIpService(fileIpService(channel->number()));
    if (!fIpService.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(_treeWidget, QObject::tr("Tano"),
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
          << indent(1) << "\"disabled\": 0," << "\n"
          << "}" << "\n";

    fIpService.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner |
                              QFile::ReadUser | QFile::WriteUser | QFile::ExeUser |
                              QFile::ReadGroup | QFile::WriteGroup | QFile::ExeGroup |
                              QFile::ReadOther | QFile::WriteOther | QFile::ExeOther);
}

QString TvheadendGenerator::indent(const int &indentLevel) const
{
    const int IndentSize = 4;
    return QString(IndentSize * indentLevel, ' ');
}
