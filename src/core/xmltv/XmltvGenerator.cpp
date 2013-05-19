/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This file was based on the example classes of the Qt Toolkit.
* Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
* Contact: Qt Software Information (qt-info@nokia.com)
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

#include <QtCore/QDate>
#include <QtCore/QDebug>
#include <QtCore/QTime>

#include "xmltv/XmltvCommon.h"
#include "xmltv/XmltvGenerator.h"
#include "xmltv/XmltvSql.h"
#include "xmltv/containers/XmltvChannel.h"
#include "xmltv/containers/XmltvCrewMember.h"
#include "xmltv/containers/XmltvProgramme.h"

XmltvGenerator::XmltvGenerator(XmltvSql *xmltv,
                               const QString &file)
{
    _xmltv = xmltv;
    _file = new QFile(file);

    int offset = QDateTime::currentDateTime().time().hour() - QDateTime::currentDateTimeUtc().time().hour();
    _timeOffset = " +0" + QString::number(offset) + "00";

    qDebug() << "Time offset:" << _timeOffset;
}

XmltvGenerator::~XmltvGenerator()
{
    delete _file;
}

bool XmltvGenerator::openFile()
{
    if (!_file->open(QFile::WriteOnly | QFile::Text))
        return false;

    _out.setDevice(_file);
    _out.setCodec("UTF-8");
    _out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
         << QString("<tv>\n");

    return true;
}

bool XmltvGenerator::writeChannels()
{
    for (int i = 0; i < _xmltv->channels().count(); ++i)
        generateChannel(_xmltv->channel(_xmltv->channels().values()[i]));

    return true;
}

bool XmltvGenerator::writeProgramme(const int &id)
{
    QList<XmltvProgramme *> list = _xmltv->programmes(_xmltv->channels().values()[id]);
    for (int i = 0; i < list.size(); ++i) {
        generateProgramme(list[i]);
    }

    return true;
}

bool XmltvGenerator::writeProgrammes()
{
    for (int i = 0; i < _xmltv->channels().count(); ++i) {
        if (!writeProgramme(i))
            return false;
    }

    return true;
}

bool XmltvGenerator::closeFile()
{
    _out << "</tv>\n";

    _file->flush();
    _file->close();
    return true;
}

QString XmltvGenerator::indent(const int &indentLevel)
{
    const int IndentSize = 4;
    return QString(IndentSize * indentLevel, ' ');
}

QString XmltvGenerator::escapedText(const QString &str)
{
    QString result = str;
    result.replace("&", "&amp;");
    result.replace("<", "&lt;");
    result.replace(">", "&gt;");
    return result;
}

QString XmltvGenerator::escapedAttribute(const QString &str)
{
    QString result = escapedText(str);
    result.replace("\"", "&quot;");
    result.prepend("\"");
    result.append("\"");
    return result;
}

void XmltvGenerator::generateChannel(XmltvChannel *channel)
{
    if (!channel)
        return;

    int depth = 1;

    _out << indent(depth) << "<channel id=" << escapedAttribute(channel->id()) << ">\n"
         << indent(depth + 1) << "<display-name>" << escapedText(channel->displayName()) << "</display-name>\n"
         << indent(depth) << "</channel>\n";

    delete channel;
}

void XmltvGenerator::generateProgramme(XmltvProgramme *programme)
{
    if (!programme)
        return;

    int depth = 1;

    _out << indent(depth) << "<programme start=" << escapedAttribute(programme->start().toString(Tano::Xmltv::dateFormat()) + _timeOffset) << " stop=" << escapedAttribute(programme->stop().toString(Tano::Xmltv::dateFormat()) + _timeOffset) << " channel=" << escapedAttribute(programme->channel()) << ">\n"
         << indent(depth + 1) << "<title>" << escapedText(programme->title()) << "</title>\n";

    if (!programme->subTitle().isEmpty())
        _out << indent(depth + 1) << "<sub-title>" << escapedText(programme->subTitle()) << "</sub-title>\n";

    foreach (QString cat, programme->categories())
        _out << indent(depth + 1) << "<category>" << escapedText(cat) << "</category>\n";

    if (!programme->desc().isEmpty())
        _out << indent(depth + 1) << "<desc>" << escapedText(programme->desc()) << "</desc>\n";

    if (!programme->icon().isEmpty())
        _out << indent(depth + 1) << "<icon src=" << escapedAttribute(programme->icon()) << " />\n";

    QList<XmltvCrewMember *> crew = _xmltv->crew(programme->id());
    if (crew.size()) {
        _out << indent(depth + 1) << "<credits>\n";
        for (int i = 0; i < crew.size(); ++i) {
            QString type = XmltvCrewMember::stringFromTypeOutput(crew[i]->type());
            QString name = crew[i]->name();
            _out << indent(depth + 2) << "<" << type << ">" << name << "</" << type << ">\n";
            delete crew[i];
        }
        _out << indent(depth + 1) << "</credits>\n";
    }

    _out << indent(depth) << "</programme>\n";

    delete programme;
}
