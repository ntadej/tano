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

#include <stdint.h>

#include <QtDBus/QDBusMetaType>
#include <QtGui/QImage>

#include "common/FreedesktopImage.h"

// Image configuration settings
const int CHANNELS = 4;
const int BYTES_PER_PIXEL = 4;
const int BITS_PER_SAMPLE = 8;

FreedesktopImage::FreedesktopImage(const QImage &img):
    _width(img.width()),
    _height(img.height()),
    _stride(img.width() * BYTES_PER_PIXEL),
    _hasAlpha(true),
    _channels(CHANNELS),
    _bitsPerSample(BITS_PER_SAMPLE)
{
    // Convert 00xAARRGGBB to RGBA bytewise (endian-independent) format
    QImage tmp = img.convertToFormat(QImage::Format_ARGB32);
    const uint32_t *data = reinterpret_cast<const uint32_t *>(tmp.bits());

    unsigned int num_pixels = _width * _height;
    _image.resize(num_pixels * BYTES_PER_PIXEL);

    for (unsigned int ptr = 0; ptr < num_pixels; ++ptr) {
        _image[ptr * BYTES_PER_PIXEL + 0] = data[ptr] >> 16; // R
        _image[ptr * BYTES_PER_PIXEL + 1] = data[ptr] >> 8;  // G
        _image[ptr * BYTES_PER_PIXEL + 2] = data[ptr];       // B
        _image[ptr * BYTES_PER_PIXEL + 3] = data[ptr] >> 24; // A
    }
}

QDBusArgument &operator<<(QDBusArgument &a,
                          const FreedesktopImage &i)
{
    a.beginStructure();
    a << i._width << i._height << i._stride << i._hasAlpha << i._bitsPerSample << i._channels << i._image;
    a.endStructure();
    return a;
}

const QDBusArgument &operator>>(const QDBusArgument &a,
                                FreedesktopImage &i)
{
    a.beginStructure();
    a >> i._width >> i._height >> i._stride >> i._hasAlpha >> i._bitsPerSample >> i._channels >> i._image;
    a.endStructure();
    return a;
}

int FreedesktopImage::metaType()
{
    return qDBusRegisterMetaType<FreedesktopImage>();
}

QVariant FreedesktopImage::toVariant(const QImage &img)
{
    FreedesktopImage fimg(img);
    return QVariant(FreedesktopImage::metaType(), &fimg);
}
