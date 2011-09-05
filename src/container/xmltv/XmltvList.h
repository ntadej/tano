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

#ifndef TANO_XMLTVLIST_H_
#define TANO_XMLTVLIST_H_

#include <QtCore/QDate>
#include <QtCore/QList>
#include <QtCore/QString>

class XmltvChannel;
class XmltvChannelsModel;

class XmltvList : public QObject
{
Q_OBJECT
public:
    XmltvList(const QDate &date = QDate(),
              QObject *parent = 0);
    ~XmltvList();

    QDate date() const { return _date; }
    QString sourceInfoUrl() const { return _sourceInfoUrl; }
    void setSourceInfoUrl(const QString &s) { _sourceInfoUrl = s; }
    QString sourceInfoName() const { return _sourceInfoName; }
    void setSourceInfoName(const QString &s) { _sourceInfoName = s; }
    QString sourceDataUrl() const { return _sourceDataUrl; }
    void setSourceDataUrl(const QString &s) { _sourceDataUrl = s; }
    QString generatorInfoName() const { return _generatorInfoName; }
    void setGeneratorInfoName(const QString &s) { _generatorInfoName = s; }
    QString generatorInfoUrl() const { return _generatorInfoUrl; }
    void setGeneratorInfoUrl(const QString &s) { _generatorInfoUrl = s; }

    XmltvChannelsModel *channels() const { return _channels; }
    void addChannel(XmltvChannel *c);

private:
    QDate _date;
    QString _sourceInfoUrl;
    QString _sourceInfoName;
    QString _sourceDataUrl;
    QString _generatorInfoName;
    QString _generatorInfoUrl;

    XmltvChannelsModel *_channels;
};

#endif // TANO_XMLTVLIST_H_
