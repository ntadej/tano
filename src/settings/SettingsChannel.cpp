/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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

#include <QtCore/QStandardPaths>

#include "application/Common.h"
#include "settings/SettingsChannel.h"

SettingsChannel::SettingsChannel(QObject *parent)
    : QSettings(SettingsChannel::fileName(),
                QSettings::IniFormat,
                parent) { }

SettingsChannel::~SettingsChannel() { }

QString SettingsChannel::fileName()
{
#if QT_VERSION < 0x050400
    return QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/ChannelSettings.ini";
#else
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/ChannelSettings.ini";
#endif
}

void SettingsChannel::setDefaults(int aspectRatio,
                                  int cropRatio,
                                  int deinterlacing,
                                  const QString &audioLanguage,
                                  const QString &subtitleLanguage)
{
    _aspectRatio = aspectRatio;
    _cropRatio = cropRatio;
    _deinterlacing = deinterlacing;
    _audioLanguage = audioLanguage;
    _subtitleLanguage = subtitleLanguage;
}

QString SettingsChannel::process(const QString &id) const
{
    QString arg = id;
    arg = arg.replace(QRegExp("\\W"), "_");

    return arg;
}

int SettingsChannel::aspectRatio(const QString &id) const
{
    return value(process(id) + "/aspectratio", _aspectRatio).toInt();
}

void SettingsChannel::setAspectRatio(const QString &id,
                                     int i)
{
    setValue(process(id) + "/aspectratio", i);
    sync();
}

int SettingsChannel::cropRatio(const QString &id) const
{
    return value(process(id) + "/cropratio", _cropRatio).toInt();
}

void SettingsChannel::setCropRatio(const QString &id,
                                   int i)
{
    setValue(process(id) + "/cropratio", i);
    sync();
}

int SettingsChannel::deinterlacing(const QString &id) const
{
    return value(process(id) + "/deinterlacing", _deinterlacing).toInt();
}

void SettingsChannel::setDeinterlacing(const QString &id,
                                       int i)
{
    setValue(process(id) + "/deinterlacing", i);
    sync();
}

QString SettingsChannel::audioLanguage(const QString &id) const
{
    return value(process(id) + "/audio", _audioLanguage).toString();
}

void SettingsChannel::setAudioLanguage(const QString &id,
                                       const QString &s)
{
    setValue(process(id) + "/audio", s);
    sync();
}

QString SettingsChannel::subtitleLanguage(const QString &id) const
{
    return value(process(id) + "/subtitles", _subtitleLanguage).toString();
}

void SettingsChannel::setSubtitleLanguage(const QString &id,
                                          const QString &s)
{
    setValue(process(id) + "/subtitles", s);
    sync();
}
