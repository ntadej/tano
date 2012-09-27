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

#ifndef TANO_XMLTVGENERATOR_H_
#define TANO_XMLTVGENERATOR_H_

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "CoreSharedExport.h"

class XmltvChannel;
class XmltvList;
class XmltvProgramme;

class TANO_CORE_EXPORT XmltvGenerator
{
public:
    XmltvGenerator(const QString &file);
    ~XmltvGenerator();

    bool openFile();
    bool writeHeader(XmltvList *xmltv);
    bool writeChannels(XmltvList *xmltv);
    bool writeProgrammes(XmltvList *xmltv);
    bool closeFile();

private:
    static QString indent(const int &indentLevel);
    static QString escapedText(const QString &str);
    static QString escapedAttribute(const QString &str);
    void generateChannel(XmltvChannel *channel);
    void generateProgramme(XmltvProgramme *programme);

    QFile *_file;
    QTextStream _out;

    QString _timeOffset;
};

#endif // TANO_XMLTVGENERATOR_H_
