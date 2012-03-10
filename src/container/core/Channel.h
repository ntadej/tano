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

#ifndef TANO_CHANNEL_H_
#define TANO_CHANNEL_H_

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "core/ListItem.h"

class Channel : public ListItem
{
Q_OBJECT
public:
    enum Roles {
        DisplayRole = Qt::DisplayRole,
        DisplayIconRole = Qt::DecorationRole,
        NameRole = Qt::UserRole + 1,
        NumberRole,
        RadioRole,
        LanguageRole,
        UrlRole,
        EpgRole,
        CategoriesRole,
        LogoRole
    };

    explicit Channel(QObject *parent = 0);
    explicit Channel(const QString &name,
                     const int &number,
                     QObject *parent = 0);
    ~Channel();

    inline QString id() const { return _url; }
    QVariant data(int role) const;
    QString display() const;
    QIcon displayIcon() const;
    QHash<int, QByteArray> roleNames() const;

    int number() const { return _number; }
    inline QString numberString() const { return QString().number(_number); }
    void setNumber(const int &number);
    inline bool radio() const { return _radio; }
    void setRadio(const bool &radio);
    inline QString name() const { return _name; }
    void setName(const QString &name);
    inline QString language() const { return _language; }
    void setLanguage(const QString &language);
    inline QString url() const { return _url; }
    void setUrl(const QString &url);
    inline QString epg() const { return _epg; }
    void setEpg(const QString &epg);
    inline QStringList categories() const { return _categories; }
    void setCategories(const QStringList &categories);
    inline QString logo() const { return _logo; }
    void setLogo(const QString &logo);

private:
    QString _name;
    int _number;
    bool _radio;
    QString _language;
    QString _url;
    QString _epg;
    QStringList _categories;
    QString _logo;
};

#endif // TANO_CHANNEL_H_
