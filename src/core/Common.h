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

#ifndef TANO_COMMON_H_
#define TANO_COMMON_H_

#include <QtCore/QDate>
#include <QtCore/QString>
#include <QtCore/QTime>

#include "CoreSharedExport.h"

namespace Tano
{
    // Tano name and executable
    TANO_CORE_EXPORT QString name();
    TANO_CORE_EXPORT QString executable();

    // Version
    TANO_CORE_EXPORT QString version();
    TANO_CORE_EXPORT QString changeset();
    TANO_CORE_EXPORT bool is64bit();
    TANO_CORE_EXPORT QString uid();

    // Info
    TANO_CORE_EXPORT QString email();
    TANO_CORE_EXPORT QString url();

    // Identifier
    enum Id
    {
        Main,
        Record,
        Schedule
    };

    // Misc
    TANO_CORE_EXPORT QString recordingFileName(const QString &name,
                                               const QString &channel,
                                               const QDate &date,
                                               const QTime &time);
}

#endif // TANO_COMMON_H_
