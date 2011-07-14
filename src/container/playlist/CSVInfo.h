/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_CSVINFO_H_
#define TANO_CSVINFO_H_

#include <QtCore/QList>
#include <QtCore/QString>

class CSVInfo
{
public:
    CSVInfo(const QString &separator = 0,
            const bool &header = false,
            const QList<int> &columns = QList<int>());
    ~CSVInfo();

    inline QString separator() const { return _separator; }
    inline bool header() const { return _header; }
    inline QList<int> columns() const { return _columns; }

private:
    QString _separator;
    bool _header;
    QList<int> _columns;
};

#endif // TANO_CSVINFO_H_
