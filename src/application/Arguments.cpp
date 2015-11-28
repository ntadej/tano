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

#include "application/Arguments.h"
#include "application/Output.h"

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

Argument Arguments::create(const Argument::Type &arg,
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
    _arguments << create(Argument::Channel, "c", "channel")
               << create(Argument::Playlist, "p", "playlist")
               << create(Argument::Xmltv, "x", "xmltv")
               << create(Argument::Aout, "a", "aout")
               << create(Argument::Vout, "v", "vout")
               << create(Argument::File, "f", "file")
               << create(Argument::Url, "u", "url");
}

bool Arguments::processArguments(const QStringList &args)
{
    int i = 1;
    while (i < args.size()) {
        if (args[i].startsWith("-")) {
            if (args[i] == "-h" || args[i] == "--help") {
                Output::help();
                return false;
            } else {
                bool done = false;
                foreach (const Argument &arg, _arguments) {
                    if (args[i] == QString("-" + arg.shortArg) ||
                        args[i] == QString("--" + arg.longArg) ||
                        args[i].startsWith("--" + arg.longArg + "="))
                    {
                        if (args[i].startsWith("--" + arg.longArg + "=")) {
                            QString exp = "--" + arg.longArg + "=";
                            QString item = args[i];
                            QString value = item.replace(exp , "");
                            setValue(arg.type, value);
                            i++;

                            done = true;
                            break;
                        } else {
                            if (i == (args.size() - 1)) {
                                Output::errorMissing(args[i]);
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
                    Output::errorUnknown(args[i]);
                    return false;
                }
            }
        } else {
            setValue(Argument::File, args[i]);
            i++;
        }
    }

    return true;
}
