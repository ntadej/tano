/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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
#include <QtCore/QTime>

#include "TimersGenerator.h"
#include "container/core/Timer.h"
#include "recorder/TimersModel.h"

TimersGenerator::TimersGenerator(const QString &file)
{
    _file = new QFile(file);
}

TimersGenerator::~TimersGenerator()
{
    delete _file;
}

bool TimersGenerator::write(TimersModel *model)
{
    if (!_file->open(QFile::WriteOnly | QFile::Text))
        return false;

    _out.setDevice(_file);
    _out.setCodec("UTF-8");
    _out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
         << "<!DOCTYPE tano>\n"
         << "<tano version=\"0.8\">\n";
    for (int i = 0; i < model->rowCount(); ++i)
        generateItem(model->row(i));
    _out << "</tano>\n";
    return true;
}

QString TimersGenerator::indent(const int &indentLevel)
{
    const int IndentSize = 4;
    return QString(IndentSize * indentLevel, ' ');
}

QString TimersGenerator::escapedText(const QString &str)
{
    QString result = str;
    result.replace("&", "&amp;");
    result.replace("<", "&lt;");
    result.replace(">", "&gt;");
    return result;
}

QString TimersGenerator::escapedAttribute(const QString &str)
{
    QString result = escapedText(str);
    result.replace("\"", "&quot;");
    result.prepend("\"");
    result.append("\"");
    return result;
}

QString TimersGenerator::boolToString(const bool &b)
{
    if(b) {
        return QString("true");
    } else {
        return QString("false");
    }
}

void TimersGenerator::generateItem(Timer *timer)
{
    int depth = 1;

    _out << indent(depth) << "<timer>\n"
         << indent(depth + 1) << "<name>" << escapedText(timer->name()) << "</name>\n"
         << indent(depth + 1) << "<playlist>" << escapedText(timer->playlist()) << "</playlist>\n"
         << indent(depth + 1) << "<channelid>" << escapedText(QString().number(timer->num())) << "</channelid>\n"
         << indent(depth + 1) << "<channel>" << escapedText(timer->channel()) << "</channel>\n"
         << indent(depth + 1) << "<url>" << escapedText(timer->url()) << "</url>\n"
         << indent(depth + 1) << "<type>" << escapedText(QString().number(timer->type())) << "</type>\n"
         << indent(depth + 1) << "<starttime>" << escapedText(timer->startTime().toString(Qt::ISODate)) << "</starttime>\n"
         << indent(depth + 1) << "<endtime>" << escapedText(timer->endTime().toString(Qt::ISODate)) << "</endtime>\n"
         << indent(depth + 1) << "<disabled>" << boolToString(timer->isDisabled()) << "</disabled>\n"
         << indent(depth) << "</timer>\n";
}
