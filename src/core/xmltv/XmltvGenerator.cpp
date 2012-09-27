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
#include "xmltv/containers/XmltvChannel.h"
#include "xmltv/containers/XmltvCrewMember.h"
#include "xmltv/containers/XmltvList.h"
#include "xmltv/containers/XmltvProgramme.h"
#include "xmltv/models/XmltvChannelsModel.h"
#include "xmltv/models/XmltvCrewModel.h"
#include "xmltv/models/XmltvProgrammeModel.h"

XmltvGenerator::XmltvGenerator(const QString &file)
{
    _file = new QFile(file);

    int offset = QDateTime::currentDateTime().time().hour() - QDateTime::currentDateTimeUtc().time().hour();
    _timeOffset = " +0" + QString::number(offset) + "00";

    qDebug() << "Time offset:" << _timeOffset;
}

XmltvGenerator::~XmltvGenerator()
{
    delete _file;
}

bool XmltvGenerator::write(XmltvList *xmltv)
{
    if (!_file->open(QFile::WriteOnly | QFile::Text))
        return false;

    _out.setDevice(_file);
    _out.setCodec("UTF-8");
    _out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
         << QString("<tv source-info-name=\"%1\" source-info-url=\"%2\" generator-info-name=\"%3\" generator-info-url=\"%4\">\n")
            .arg(xmltv->sourceInfoName(), xmltv->sourceInfoUrl(), xmltv->generatorInfoName(), xmltv->generatorInfoUrl());
    for (int i = 0; i < xmltv->channels()->rowCount(); ++i)
        generateChannel(xmltv->channels()->row(i));
    for (int i = 0; i < xmltv->channels()->rowCount(); ++i) {
        for (int j = 0; j < xmltv->channels()->row(i)->programme()->rowCount(); ++j) {
            generateProgramme(xmltv->channels()->row(i)->programme()->row(j));
        }
    }
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
    int depth = 1;

    _out << indent(depth) << "<channel id=" << escapedAttribute(channel->id()) << ">\n"
         << indent(depth + 1) << "<display-name>" << escapedText(channel->displayName()) << "</display-name>\n"
         << indent(depth) << "</channel>\n";
}

void XmltvGenerator::generateProgramme(XmltvProgramme *programme)
{
    int depth = 1;

    _out << indent(depth) << "<programme start=" << escapedAttribute(programme->start().toString(Tano::Xmltv::dateFormat()) + _timeOffset) << " stop=" << escapedAttribute(programme->stop().toString(Tano::Xmltv::dateFormat()) + _timeOffset) << " channel=" << escapedAttribute(programme->channel()) << ">\n"
         << indent(depth + 1) << "<title>" << escapedText(programme->title()) << "</title>\n";

    foreach (QString cat, programme->categories())
        _out << indent(depth + 1) << "<category>" << escapedText(cat) << "</category>\n";

    if (!programme->desc().isEmpty())
        _out << indent(depth + 1) << "<desc>" << escapedText(programme->desc()) << "</desc>\n";

    if (!programme->icon().isEmpty())
        _out << indent(depth + 1) << "<icon>" << escapedText(programme->icon()) << "</icon>\n";

    if (programme->crew()->rowCount()) {
        _out << indent(depth + 1) << "<credits>\n";
        for (int i = 0; i < programme->crew()->rowCount(); ++i) {
            QString type = XmltvCrewMember::stringFromTypeOutput(programme->crew()->row(i)->type());
            QString name = programme->crew()->row(i)->name();
            _out << indent(depth + 2) << "<" << type << ">" << name << "</" << type << ">\n";
        }
        _out << indent(depth + 1) << "</credits>\n";
    }

    _out << indent(depth) << "</programme>\n";
}
