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

#ifndef TANO_TIMER_H_
#define TANO_TIMER_H_

#include <QtCore/QDateTime>
#include <QtCore/QString>

#include "core/Enums.h"
#include "core/ListItem.h"

class Timer : public ListItem
{
Q_OBJECT
public:
    enum Roles {
        DisplayRole = Qt::DisplayRole,
        DisplayIconRole = Qt::DecorationRole,
        NameRole = Qt::UserRole + 1,
        ChannelRole,
        PlaylistRole,
        UrlRole,
        NumRole,
        StartTimeRole,
        EndTimeRole,
        DisabledRole,
        TypeRole,
        RecordingRole
    };

    Timer(const QString &name = 0,
          const QString &channel = 0,
          const QString &playlist = 0,
          const int &num = 0,
          const QString &url = 0,
          QObject *parent = 0);
    ~Timer();


    inline QString id() const { return _name; }
    QVariant data(int role) const;
    QString display() const;
    QIcon displayIcon() const;
    QHash<int, QByteArray> roleNames() const;

    inline QString name() const { return _name; }
    void setName(const QString &name);
    inline QString channel() const { return _channel; }
    void setChannel(const QString &channel);
    inline QString playlist() const { return _playlist; }
    void setPlaylist(const QString &playlist);
    inline int num() const { return _num; }
    void setNum(const int &num);
    inline QString url() const { return _url; }
    void setUrl(const QString &url);
    inline QDateTime startTime() const { return _startTime; }
    void setStartTime(const QDateTime &startTime);
    inline QDateTime endTime() const { return _endTime; }
    void setEndTime(const QDateTime &endTime);
    inline bool isDisabled() const { return _disabled; }
    void setDisabled(const bool &disabled);
    inline Tano::TimerType type() const { return _type; }
    void setType(const Tano::TimerType &type);
    inline bool isRecording() const { return _recording; }
    void setRecording(const bool &recording);

private:
    QString _name;
    QString _channel;
    QString _playlist;
    int _num;
    QString _url;
    QDateTime _startTime;
    QDateTime _endTime;
    bool _disabled;
    Tano::TimerType _type;
    bool _recording;
};

#endif // TANO_TIMER_H_
