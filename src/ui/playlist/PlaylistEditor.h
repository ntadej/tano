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

#ifndef TANO_PLAYLISTEDITOR_H_
#define TANO_PLAYLISTEDITOR_H_

#include <QtGui/QMainWindow>

#include "Config.h"

class Channel;
class PlaylistModel;

#if EDITOR
class LocaleManager;
#if UPDATE
class UpdateDialog;
#endif
#endif

class VlcInstance;
#if WITH_EDITOR_VLCQT
class PlaylistEditorScan;
#endif

namespace Ui
{
    class PlaylistEditor;
}

class PlaylistEditor : public QMainWindow
{
Q_OBJECT
public:
    PlaylistEditor(QWidget *parent = 0);
    ~PlaylistEditor();

    void setMediaInstance(VlcInstance *instance);

public slots:
    void newPlaylist();
    void open(const QString &playlist = 0,
              const bool &refresh = false);

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

private slots:
    void aboutTano();
    void help();
    void settings();
    void setTitle(const QString &title);

    void deleteItem();
    void addItem();
    void addItem(const QString &name,
                 const QString &url);

    void save();
    void exportTvheadend();
    void exportXmltvId();
    void print();

    void menuOpenExport();
    void exit();

    void editItem(Channel *channel);

    void editChannelNumber();
    void editChannelRadio(const bool &radio);
    void editChannelName(const QString &text);
    void editChannelUrl(const QString &text);
    void editChannelCategories(const QString &text);
    void editChannelLanguage(const QString &text);
    void editChannelXmltvId(const QString &text);
    void editChannelLogo(const QString &text);

    void moveUp();
    void moveDown();

    void scan(const bool &status);
    void updateAvailable();

private:
    void createConnections();
    void createSettings();

    Ui::PlaylistEditor *ui;

    PlaylistModel *_model;

#if EDITOR
    LocaleManager *_locale;
#endif

    bool _closeEnabled;
    QMenu *_menuExport;

#if EDITOR && UPDATE
    UpdateDialog *_update;
#endif

#if WITH_EDITOR_VLCQT
    PlaylistEditorScan *_scan;
#endif
};

#endif // TANO_PLAYLISTEDITOR_H_
