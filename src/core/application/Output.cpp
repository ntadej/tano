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

void Output::help()
{
#if defined(Q_OS_WIN32)
    Tano::Windows::showConsole();
#endif

    Output() << QObject::tr("Usage: %1 [options]").arg(Tano::executable()) << endl << endl;

    Output() << QObject::tr("IP TV playback:") << endl
          << "  " << "-c, --channel <number>" << "\t" << QObject::tr("Play channel by number") << endl
          << "  " << "-p, --playlist <file>" << "\t\t" << QObject::tr("Use playlist file") << endl
          << "  " << "-x, --xmltv <file>" << "\t\t" << QObject::tr("Use XMLTV file") << endl
          << endl;

    Output() << QObject::tr("Open:") << endl
          << "  " << "-f, --file <file>" << "\t\t" << QObject::tr("Open file") << endl
          << "  " << "-u, --url <url>" << "\t\t" << QObject::tr("Open URL") << endl
          << endl;

    Output() << QObject::tr("Playback options:") << endl
          << "  " << "-a, --aout <name>" << "\t\t" << QObject::tr("Set audio output") << endl
          << "  " << "-v, --vout <name>" << "\t\t" << QObject::tr("Set video output") << endl
          << endl;

#if defined(Q_OS_WIN32)
    Tano::Windows::pauseConsole();
#endif
}

void Output::errorGeneric()
{
    Output() << QObject::tr("Try '%1 %2' for more information.").arg(Tano::executable(), "--help") << endl;
}

void Output::errorMissing(const QString &arg)
{
#if defined(Q_OS_WIN32)
    Tano::Windows::showConsole();
#endif

    Output() << QObject::tr("Missing mandatory argument for '%1'.").arg(arg) << endl;
    Output::errorGeneric();

#if defined(Q_OS_WIN32)
    Tano::Windows::pauseConsole();
#endif
}

void Output::errorUnknown(const QString &arg)
{
#if defined(Q_OS_WIN32)
    Tano::Windows::showConsole();
#endif

    Output() << QObject::tr("Unknown option '%1'.").arg(arg) << endl;
    Output::errorGeneric();

#if defined(Q_OS_WIN32)
    Tano::Windows::pauseConsole();
#endif
}
