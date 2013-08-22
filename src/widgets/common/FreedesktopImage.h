/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
*
* This file was part of Bitcoin.
* Copyright (C) 2013 Bitcoin Developers
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

#ifndef TANO_FREEDESKTOPIMAGE_H_
#define TANO_FREEDESKTOPIMAGE_H_

#include <QtDBus/QDBusArgument>

#define FREEDESKTOP_NOTIFICATION_ICON_SIZE 128

// Loosely based on http://www.qtcentre.org/archive/index.php/t-25879.html
class FreedesktopImage
{
public:
    FreedesktopImage() {}
    FreedesktopImage(const QImage &img);

    static int metaType();

    // Image to variant that can be marshalled over DBus
    static QVariant toVariant(const QImage &img);

private:
    int _width, _height, _stride;
    bool _hasAlpha;
    int _channels;
    int _bitsPerSample;
    QByteArray _image;

    friend QDBusArgument &operator<<(QDBusArgument &a,
                                     const FreedesktopImage &i);
    friend const QDBusArgument &operator>>(const QDBusArgument &a,
                                           FreedesktopImage &i);
};

Q_DECLARE_METATYPE(FreedesktopImage)

#endif // TANO_FREEDESKTOPIMAGE_H_
