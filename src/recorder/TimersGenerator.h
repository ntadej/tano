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

#ifndef TANO_TIMERSGENERATOR_H_
#define TANO_TIMERSGENERATOR_H_

#include <QtCore/QFile>
#include <QtCore/QTextStream>

class Timer;
class TimersModel;

class TimersGenerator
{
public:
    TimersGenerator(const QString &file);
    ~TimersGenerator();

    bool write(TimersModel *model);

private:
    static QString indent(const int &indentLevel);
    static QString escapedText(const QString &str);
    static QString escapedAttribute(const QString &str);
    void generateItem(Timer *timer);

    QFile *_file;
    QTextStream _out;
};

#endif // TANO_TIMERSGENERATOR_H_
