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

#ifndef TANO_ARGUMENTS_H_
#define TANO_ARGUMENTS_H_

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "CoreSharedExport.h"

struct Argument
{
    // Argument types
    enum Type
    {
        Channel,
        Playlist,
        Xmltv,
        Aout,
        Vout,
        File,
        Url
    };

    Type type;
    QString shortArg;
    QString longArg;
};

class TANO_CORE_EXPORT Arguments
{
public:
    Arguments(int argc,
              char *argv[]);
    Arguments(const QStringList &args);
    ~Arguments();

    bool isValid() const { return _valid; }
    QString value(const Argument::Type &arg) const { return _values[arg]; }

private:
    Argument create(const Argument::Type &arg,
                    const QString &shortArg,
                    const QString &longArg) const;
    void createArguments();
    bool processArguments(const QStringList &args);
    void setValue(const Argument::Type &arg,
                  const QString &value) { _values[arg] = value; }

    QList<Argument> _arguments;
    QStringList _values;

    bool _valid;
};

#endif // TANO_ARGUMENTS_H_
