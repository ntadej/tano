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

#ifndef TANO_UPDATEINFO_H_
#define TANO_UPDATEINFO_H_

#include <QtCore/QString>

class UpdateInfo
{
public:
    UpdateInfo(const QString &name = 0);
    ~UpdateInfo();

    QString name() const { return _name; }
    void setName(const QString &name) { _name = name; }
    QString version() const { return _version; }
    void setVersion(const QString &version) { _version = version; }
    QString date() const { return _date; }
    void setDate(const QString &date) { _date = date; }
    QString description() const { return _description; }
    void setDescription(const QString &description) { _description = description; }
    bool development() const { return _development;}
    void setDevelopment(const bool &development) { _development = development; }

private:
    QString _name;
    QString _version;
    QString _date;
    QString _description;
    bool _development;
};

#endif // TANO_UPDATEINFO_H_
