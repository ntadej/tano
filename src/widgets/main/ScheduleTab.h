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

#ifndef TANO_SCHEDULETAB_H_
#define TANO_SCHEDULETAB_H_

#include <QtCore/QDate>
#include <QtCore/QModelIndex>

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QMainWindow>
#else
    #include <QtGui/QMainWindow>
#endif

class QComboBox;
class QLabel;
class QListView;
class QStackedWidget;
class QPushButton;
class QToolBar;

class Channel;
class FancyLineEdit;
class PlaylistDisplayWidget;
class XmltvProgrammeFilterModel;
class XmltvProgrammeModel;

class ScheduleTab : public QMainWindow
{
Q_OBJECT
public:
    explicit ScheduleTab(QWidget *parent = 0);
    ~ScheduleTab();

    inline PlaylistDisplayWidget *playlist() { return _playlistWidget; }

protected:
    void changeEvent(QEvent *e);

public slots:
    void reset();
    void setEpg(const QString &channel,
                XmltvProgrammeModel *epg);
    void setPage(const int &id);

signals:
    void itemSelected(const QString &);
    void requestEpg(const QString &);
    void requestRecord(const QString &);
    
private slots:
    void change();
    void channel(Channel *channel);
    void info();
    void programmeClicked(const QModelIndex &index);
    void processFilters();
    void record();
    void showMenu(const QPoint &pos);

private:
    XmltvProgrammeModel *_model;
    XmltvProgrammeFilterModel *_filterModel;

    PlaylistDisplayWidget *_playlistWidget;

    QStackedWidget *_main;
    QListView *_view;

    QToolBar *_toolbarTop;
    QToolBar *_toolbarBottomType;
    QToolBar *_toolbarBottomSearch;

    QLabel *_labelTitle;

    QWidget *_noEpg;
    QLabel *_noEpgIcon;
    QLabel *_noEpgLabel;
    QPushButton *_noEpgButton;

    QComboBox *_selectDate;
    FancyLineEdit *_search;

    QAction *_change;
    QAction *_info;
    QAction *_record;
    QMenu *_rightMenu;
    QPoint _currentPos;
};

#endif // TANO_SCHEDULETAB_H_
