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

#ifndef TANO_SETTINGSDIALOG_H_
#define TANO_SETTINGSDIALOG_H_

#include <QtWidgets/QActionGroup>
#include <QtWidgets/QMainWindow>

#define MAC_NATIVE_TOOLBAR 1

#if defined(Q_OS_MAC) && MAC_NATIVE_TOOLBAR
    #include <QtMacExtras>
#endif

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QMainWindow
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

public slots:
    void refreshAndShow();

protected:
    void changeEvent(QEvent *e);

signals:
    void generalChanged();
    void localeChanged();
    void guiChanged();
    void channelsChanged();
    void udpxyChanged();
    void playbackRememberChanged();
    void playbackDefaultsChanged();
    void playbackMiscChanged();
    void xmltvChanged();

private slots:
    void actionToggled();
    void localeSettingsChanged();
    void generalSettingsChanged();
    void guiSettingsChanged();

private:
    Ui::SettingsDialog *ui;

    QActionGroup *_group;

    void loadLocales();
    QStringList _locales;

#if defined(Q_OS_MAC) && MAC_NATIVE_TOOLBAR
    QMacToolBar *_macToolbar;
    QMacToolBarItem *_macItemGeneral;
    QMacToolBarItem *_macItemTv;
    QMacToolBarItem *_macItemPlayback;
    QMacToolBarItem *_macItemInterface;
    QMacToolBarItem *_macItemNotifications;
    QMacToolBarItem *_macItemShortcuts;
#endif
};

#endif // TANO_SETTINGSDIALOG_H_
