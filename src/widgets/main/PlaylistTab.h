/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_PLAYLISTTAB_H_
#define TANO_PLAYLISTTAB_H_

#include <QMainWindow>

class QComboBox;
class QLabel;
class QToolBar;

class FancyLineEdit;
class PlaylistDisplayWidget;

class PlaylistTab : public QMainWindow
{
Q_OBJECT
public:
    explicit PlaylistTab(QWidget *parent = 0);
    ~PlaylistTab();

    inline PlaylistDisplayWidget *playlist() { return _playlistWidget; }
    void setFilters(const QStringList &categories,
                    const QStringList &languages);
    void setPlaylistName(const QString &name);

protected:
    void changeEvent(QEvent *e);

signals:
    void changeTo(QWidget *);
    
private slots:
    void processFilters();

private:
    PlaylistDisplayWidget *_playlistWidget;

    QToolBar *_toolbarTop;
    QToolBar *_toolbarBottomType;
    QToolBar *_toolbarBottomSearch;

    QLabel *_labelPlaylist;

    QComboBox *_selectType;
    QComboBox *_selectCategory;
    QComboBox *_selectLanguage;

    FancyLineEdit *_search;
};

#endif // TANO_PLAYLISTTAB_H_
