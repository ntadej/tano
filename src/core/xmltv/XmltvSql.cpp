/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#include <QtCore/QDebug>
#include <QtCore/QStringList>

#include "XmltvSql.h"

#include "core/Resources.h"
#include "xmltv/XmltvCommon.h"
#include "xmltv/containers/XmltvChannel.h"
#include "xmltv/containers/XmltvCrewMember.h"
#include "xmltv/containers/XmltvProgramme.h"
#include "xmltv/models/XmltvCrewModel.h"

XmltvSql::XmltvSql()
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(Tano::Resources::settingsPath() + "/xmltv.db");
}

XmltvSql::XmltvSql(const QString &name,
                   const QString &location)
{
    _db = QSqlDatabase::addDatabase("QSQLITE", name);
    _db.setDatabaseName(location + "/" + name + ".db");
}

XmltvSql::~XmltvSql()
{
    close();
}

void XmltvSql::clean()
{
    QDateTime time(QDate::currentDate(), QTime(0, 0));

    QSqlQuery q = query();
    q.exec("DELETE FROM `programmes`"
           "WHERE `start` < " + QString::number(time.toTime_t()));
    q.exec("DELETE FROM `crew`"
           "WHERE `start` < " + QString::number(time.toTime_t()));
}

void XmltvSql::close()
{
    _db.close();
}

bool XmltvSql::open()
{
    if (!_db.open()) {
        qCritical() << "Cannot open database:" << "XMLTV";
        return false;
    } else {
        qDebug() << "Connected to database:" << "XMLTV";
    }

    QSqlQuery q = query();
    q.exec("CREATE TABLE IF NOT EXISTS `channels` ("
           "`id` varchar(40) PRIMARY KEY, "
           "`displayName` varchar(50), "
           "`icon` varchar(100), "
           "`url` varchar(100)"
           ")");

    q.exec("CREATE TABLE IF NOT EXISTS `programmes` ("
           "`id` varchar(50) PRIMARY KEY, "
           "`title` varchar(50), "
           "`subtitle` varchar(50), "
           "`channel` varchar(40), "
           "`start` varchar(20), "
           "`stop` varchar(20), "
           "`desc` text, "
           "`date` varchar(20), "
           "`categories` varchar(50), "
           "`language` varchar(50), "
           "`originalLanguage` varchar(50), "
           "`length` varchar(50), "
           "`lengthunits` varchar(50), "
           "`icon` varchar(50), "
           "`iconsize` varchar(50)"
           ")");

    q.exec("CREATE TABLE IF NOT EXISTS `crew` ("
           "`id` varchar(50) PRIMARY KEY, "
           "`name` varchar(100), "
           "`type` int, "
           "`programme` varchar(50), "
           "`start` varchar(20)"
           ")");

    qDebug() << "Available tables:" << _db.tables();

    return true;
}

QSqlQuery XmltvSql::query()
{
    return QSqlQuery(_db);
}

void XmltvSql::addChannel(XmltvChannel *channel)
{
    QSqlQuery q = query();
    q.prepare("INSERT INTO `channels`"
              "VALUES (:id, :displayName, :icon, :url)");
    q.bindValue(":id", channel->id());
    q.bindValue(":displayName", channel->displayName());
    q.bindValue(":icon", channel->icon());
    q.bindValue(":url", channel->url());
    q.exec();

    delete channel;
}

void XmltvSql::updateChannel(const QString &id,
                             const QString &column,
                             const QString &data)
{
    QSqlQuery q = query();
    q.exec("UPDATE `channels` SET `" + column + "` = '" + data + "' WHERE `id` = '" + id + "'");
}

void XmltvSql::addProgramme(XmltvProgramme *programme)
{
    QSqlQuery q = query();
    q.prepare("INSERT INTO `programmes`"
              "VALUES (:id, :title, :subtitle, :channel, :start, :stop, "
              ":desc, :date, :categories, :language, :originalLanguage, "
              ":length, :lengthunits, :icon, :iconsize)");
    q.bindValue(":id", programme->id());
    q.bindValue(":title", programme->title());
    q.bindValue(":subtitle", programme->subTitle());
    q.bindValue(":channel", programme->channel());
    q.bindValue(":start", programme->start());
    q.bindValue(":stop", programme->stop());
    q.bindValue(":desc", programme->desc());
    q.bindValue(":date", programme->date().toString(Tano::Xmltv::dateFormat()));
    q.bindValue(":categories", programme->categories().join(Tano::Xmltv::commaSeparator()));
    q.bindValue(":language", programme->language());
    q.bindValue(":originalLanguage", programme->originalLanguage());
    q.bindValue(":length", programme->length());
    q.bindValue(":lengthunits", programme->lengthUnits());
    q.bindValue(":icon", programme->icon());
    q.bindValue(":iconsize", programme->iconSize());
    q.exec();
}

void XmltvSql::updateProgramme(const QString &id,
                               const QString &column,
                               const QString &data)
{
    QSqlQuery q = query();
    q.exec("UPDATE `programmes` SET `" + column + "` = '" + data + "' WHERE `id` = '" + id + "'");
}

void XmltvSql::addCrewMember(XmltvCrewMember *member)
{
    QSqlQuery q = query();
    q.prepare("INSERT INTO `crew`"
              "VALUES (:id, :name, :type, :programme, :start)");
    q.bindValue(":id", member->id());
    q.bindValue(":name", member->name());
    q.bindValue(":type", member->type());
    q.bindValue(":programme", member->programme());
    q.bindValue(":start", member->start());
    q.exec();

    delete member;
}

XmltvChannel *XmltvSql::channel(const QString &id)
{
    QSqlQuery q = query();
    q.exec("SELECT * FROM `channels` WHERE `id` = '" + id + "'");
    if (q.next()) {
        XmltvChannel *channel = new XmltvChannel(id);
        channel->setDisplayName(q.value(1).toString());
        channel->setIcon(q.value(2).toString());
        channel->setUrl(q.value(3).toString());
        return channel;
    }

    return 0;
}

QHash<QString, QString> XmltvSql::channels()
{
    QHash<QString, QString> hash;

    QSqlQuery q = query();
    q.exec("SELECT * FROM `channels`");
    while (q.next()) {
        hash.insert(q.value(1).toString(), q.value(0).toString());
    }

    return hash;
}

QList<XmltvCrewMember *> XmltvSql::crew(const QString &programme)
{
    QSqlQuery q = query();
    q.exec("SELECT * FROM `crew` WHERE `programme` = '" + programme + "'");
    QList<XmltvCrewMember *> list;
    while (q.next()) {
        XmltvCrewMember *member = new XmltvCrewMember(
                    q.value(1).toString(),
                    XmltvCrewMember::Type(q.value(2).toInt()),
                    q.value(3).toString(),
                    q.value(4).toInt());
        list << member;
    }

    return list;
}

XmltvProgramme *XmltvSql::programme(const QString &id)
{
    QSqlQuery q = query();
    q.exec("SELECT * FROM `programmes` AS p LEFT OUTER JOIN `channels` AS c ON p.channel = c.id WHERE p.`id` = '" + id + "'");
    if (q.next()) {
        XmltvProgramme *programme = new XmltvProgramme();
        programme->setTitle(q.value(1).toString());
        programme->setSubTitle(q.value(2).toString());
        programme->setChannel(q.value(3).toString());
        programme->setStart(q.value(4).toInt());
        programme->setStop(q.value(5).toInt());
        programme->setDesc(q.value(6).toString());
        programme->setDate(QDateTime::fromString(q.value(7).toString(), Tano::Xmltv::dateFormat()));

        QString categories = q.value(8).toString();
        if (categories.isEmpty())
            programme->setCategories(QStringList());
        else
            programme->setCategories(categories.split(Tano::Xmltv::commaSeparator()));

        programme->setLanguage(q.value(9).toString());
        programme->setOriginalLanguage(q.value(10).toString());
        programme->setLength(q.value(11).toString());
        programme->setLengthUnits(XmltvProgramme::LengthUnits(q.value(12).toInt()));
        programme->setIcon(q.value(13).toString());
        programme->setIconSize(q.value(14).toSize());
        programme->setChannelDisplayName(q.value(16).toString());

        programme->setCrew(new XmltvCrewModel(programme->id(), this, programme));

        return programme;
    }

    return 0;
}

QList<XmltvProgramme *> XmltvSql::programmes(const QString &channel)
{
    QSqlQuery q = query();
    q.exec("SELECT * FROM `programmes` WHERE `channel` = '" + channel + "'");
    QList<XmltvProgramme *> list;
    while (q.next()) {
        list << programme(q.value(0).toString());
    }

    return list;
}

XmltvProgramme *XmltvSql::programmeCurrent(const QString &id)
{
    QSqlQuery q = query();
    q.exec("SELECT * FROM `programmes` WHERE `channel` = '" + id + "' AND `stop` > " + QString::number(QDateTime::currentDateTime().toTime_t()) + " ORDER BY `start`");
    if (q.next()) {
        return programme(q.value(0).toString());
    }

    return 0;
}

QStringList XmltvSql::programmeCurrentDisplay(const QString &id)
{
    QString output = "<a href=\"%1\">%2 - %3</a>";
    QStringList epg;
    int c = 0;

    QSqlQuery q = query();
    q.exec("SELECT * FROM `programmes` WHERE `channel` = '" + id + "' AND `stop` > " + QString::number(QDateTime::currentDateTime().toTime_t()) + " ORDER BY `start`");
    while (q.next() && c < 2) {
        epg << output.arg(q.value(0).toString(), QDateTime::fromTime_t(q.value(4).toInt()).toString(Tano::Xmltv::timeFormatDisplay()), q.value(1).toString());
        c++;
    }

    return epg;
}

XmltvProgramme *XmltvSql::programmeNext(const QString &id,
                                        const QString &channel)
{
    QSqlQuery q = query();
    q.exec("SELECT * FROM `programmes` WHERE `channel` = '" + channel + "' AND `id` > '" + id + "'");
    if (q.next()) {
        return programme(q.value(0).toString());
    }

    return 0;
}

XmltvProgramme *XmltvSql::programmePrevious(const QString &id,
                                            const QString &channel)
{
    QSqlQuery q = query();
    q.exec("SELECT * FROM `programmes` WHERE `channel` = '" + channel + "' AND `id` < '" + id + "' ORDER BY `id` DESC");
    if (q.next()) {
        return programme(q.value(0).toString());
    }

    return 0;
}

bool XmltvSql::startTransaction()
{
    return _db.transaction();
}

bool XmltvSql::endTransaction()
{
    return _db.commit();
}
