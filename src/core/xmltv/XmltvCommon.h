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

#ifndef TANO_XMLTVCOMMON_H_
#define TANO_XMLTVCOMMON_H_

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "CoreSharedExport.h"

namespace Tano
{
    /*!
        \namespace Xmltv
        \brief Common static functions for XMLTV
    */
    namespace Xmltv
    {
        /*!
            \brief Comma separator
            \return separator (QString)
        */
        TANO_CORE_EXPORT QString commaSeparator();

        /*!
            \brief Date format for storage
            \return format (QString)
        */
        TANO_CORE_EXPORT QString dateFormat();
        /*!
            \brief Date format for display
            \return format (QString)
        */
        TANO_CORE_EXPORT QString dateFormatDisplay();
        /*!
            \brief Parse date
            \param data date in XMLTV string format (QString)
            \return date timestamp in seconds (int)
        */
        TANO_CORE_EXPORT int parse(const QString &data);
        /*!
            \brief Time format for display
            \return format (QString)
        */
        TANO_CORE_EXPORT QString timeFormatDisplay();

        /*!
            \brief Common category-translation map
            \return category map (QMap<QString, QString>)
        */
        TANO_CORE_EXPORT QMap<QString, QString> categoryMap();

        /*!
            \brief Map category to translation
            \param cat category to map (QString)
            \return mapped category (QString)
        */
        TANO_CORE_EXPORT QString mapCategory(const QString &cat);

        /*!
            \brief Get current timezone difference
            \return timezone difference in seconds (int)
        */
        TANO_CORE_EXPORT int timeZoneDiff();
    }
}

#endif // TANO_XMLTVCOMMON_H_
