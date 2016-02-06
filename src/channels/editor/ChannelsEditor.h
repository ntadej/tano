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

#ifndef TANO_CHANNELSEDITOR_H_
#define TANO_CHANNELSEDITOR_H_

#include <QtCore/QHash>
#include <QtWidgets/QMainWindow>

class Channel;
class ChannelsModel;

class VlcInstance;

namespace Ui
{
    class ChannelsEditor;
}

class ChannelsEditor : public QMainWindow
{
    Q_OBJECT
public:
    ChannelsEditor(QWidget *parent = 0);
    ~ChannelsEditor();

    void setMediaInstance(VlcInstance *instance);

public slots:
    void newChannels();
    void open(const QString &channelsList = 0,
              bool refresh = false);
    void setXmltvMap(const QHash<QString, QString> &map);

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

private slots:
    void help();
    void setTitle(const QString &title = "");

    void deleteItem();
    void addItem();
    void addItem(const QString &name,
                 const QString &url);

    void save();
    void exportXmltvId();
    void print();

    void menuOpenExport();
    void exit();

    void editItem(Channel *channel);

    void editChannelNumber();
    void editChannelType(int type);
    void editChannelName(const QString &text);
    void editChannelUrl(const QString &text);
    void editChannelCategories(const QString &text);
    void editChannelLanguage(const QString &text);
    void editChannelXmltvId(const QString &text);
    void editChannelLogo(const QString &text);

    void moveUp();
    void moveDown();

    void scan(bool status);

    void autoMapXmltv();

private:
    void createConnections();

    void mapXmltv();

    Ui::ChannelsEditor *ui;

    ChannelsModel *_model;

    bool _closeEnabled;
    QMenu *_menuExport;

    QHash<QString, QString> _map;
};

#endif // TANO_CHANNELSEDITOR_H_
