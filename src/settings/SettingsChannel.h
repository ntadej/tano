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

#ifndef TANO_SETTINGSCHANNEL_H_
#define TANO_SETTINGSCHANNEL_H_

#include <QtCore/QSettings>

class SettingsChannel : public QSettings
{
public:
    SettingsChannel(QObject *parent = 0);
    ~SettingsChannel();

    static QString fileName();

    void setDefaults(int aspectRatio,
                     int cropRatio,
                     int deinterlacing,
                     const QString &audioLanguage,
                     const QString &subtitleLanguage);

    int aspectRatio(const QString &id) const;
    int cropRatio(const QString &id) const;
    int deinterlacing(const QString &id) const;
    QString audioLanguage(const QString &id) const;
    QString subtitleLanguage(const QString &id) const;

public slots:
    void setAspectRatio(const QString &id,
                        int i);
    void setCropRatio(const QString &id,
                      int i);
    void setDeinterlacing(const QString &id,
                          int i);
    void setAudioLanguage(const QString &id,
                          const QString &s);
    void setSubtitleLanguage(const QString &id,
                             const QString &s);

private:
    QString process(const QString &id) const;

    int _aspectRatio;
    int _cropRatio;
    int _deinterlacing;
    QString _audioLanguage;
    QString _subtitleLanguage;
};

#endif // TANO_SETTINGSCHANNEL_H_
