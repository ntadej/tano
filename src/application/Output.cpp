/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
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
#include <QtCore/QTextCodec>

#include "Common.h"
#include "Output.h"

#if defined(Q_OS_WIN32)
#   include "platform/Windows.h"
#endif

Output::Output(bool error)
    : QTextStream(error ? stderr : stdout, QIODevice::WriteOnly) { }

Output::~Output() { }

Output& Output::operator<<(const QString &string)
{
    QTextStream::operator<<(string);

    flush();

    return *this;
}

void Output::welcome()
{
    Output() << QString("%1 %2").arg(Tano::name(), Tano::version()) << endl
          << QString("Copyright (C) %1 Tadej Novak").arg(QDate::currentDate().year()) << endl << endl;
}
