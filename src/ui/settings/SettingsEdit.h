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

#ifndef TANO_SETTINGSEDIT_H_
#define TANO_SETTINGSEDIT_H_

#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtGui/QAbstractButton>
#include <QtGui/QDialog>

class Settings;
class Shortcuts;

namespace Ui
{
    class SettingsEdit;
}

class SettingsEdit : public QDialog
{
Q_OBJECT
public:
    SettingsEdit(Shortcuts *s = 0,
                 QWidget *parent = 0);
    ~SettingsEdit();

protected:
    void changeEvent(QEvent *e);

private slots:
    void action(QAbstractButton *button);
    void apply();
    void cancel();
    void save();

    void recorderDirectoryBrowse();
    void recorderDirectoryReset();

private:
    void createActions();
    void loadLocale();
    void read();

    Ui::SettingsEdit *ui;

    Settings *_settings;

    QStringList _locale;
};

#endif // TANO_SETTINGSEDIT_H_
