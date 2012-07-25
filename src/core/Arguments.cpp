/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

#if defined(Qt5)
    #include <QtWidgets/QApplication>
#elif defined(Qt4)
    #include <QtGui/QApplication>
#endif

#include "core/Arguments.h"
#include "core/Out.h"

Arguments::Arguments(int argc, char *argv[])
{
    QStringList args;
    for (int i = 0; i < argc; i++) {
        args << QString(argv[i]);
    }

    createArguments();

    foreach (const Argument &arg, _arguments) {
        Q_UNUSED(arg)

        _values << QString();
    }

    _valid = processArguments(args);
}

Arguments::Arguments(const QStringList &args)
{
    processArguments(args);
}

Arguments::~Arguments() { }

Argument Arguments::create(const Tano::Argument &arg,
                           const QString &shortArg,
                           const QString &longArg) const
{
    Argument a;
    a.type = arg;
    a.shortArg = shortArg;
    a.longArg = longArg;

    return a;
}

void Arguments::createArguments()
{
    _arguments << create(Tano::AEditor, "e", "editor")
               << create(Tano::AChannel, "c", "channel")
               << create(Tano::APlaylist, "p", "playlist")
               << create(Tano::ARecord, "r", "record")
               << create(Tano::AXmltv, "x", "xmltv")
               << create(Tano::AAout, "a", "aout")
               << create(Tano::AVout, "v", "vout")
               << create(Tano::AFile, "f", "file");
}

bool Arguments::processArguments(const QStringList &args)
{
    int i = 1;
    while (i < args.size()) {
        if (args[i].startsWith("-")) {
            if (args[i] == "-h" || args[i] == "--help") {
                // display help
                return false;
            } else {
                bool done = false;
                foreach (const Argument &arg, _arguments) {
                    if (args[i] == QString("-" + arg.shortArg) ||
                        args[i] == QString("--" + arg.longArg) ||
                        args[i].startsWith("--" + arg.longArg + "="))
                    {
                        if (arg.type == Tano::AEditor) {
                            setValue(arg.type, "true");
                            i++;

                            done = true;
                            break;
                        } else if (args[i].startsWith("--" + arg.longArg + "=")) {
                            QString exp = "--" + arg.longArg + "=";
                            QString item = args[i];
                            QString value = item.replace(exp , "");
                            setValue(arg.type, value);
                            i++;

                            done = true;
                            break;
                        } else {
                            if (i == (args.size() - 1)) {
                                // invalid
                                return false;
                            }

                            setValue(arg.type, args[i + 1]);
                            i+=2;

                            done = true;
                            break;
                        }
                    }
                }

                if (!done) {
                    // invalid
                    return false;
                }
            }
        } else {
            // open file
        }
    }

    return true;
}
