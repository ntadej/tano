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

#ifndef TANO_SETTINGSEDIT_H_
#define TANO_SETTINGSEDIT_H_

#if defined(Qt5)
    #include <QtWidgets/QDialog>
#elif defined(Qt4)
    #include <QtGui/QDialog>
#endif

class QAbstractButton;
class QButtonGroup;

class Settings;

namespace Ui
{
    class SettingsEditorDialog;
}

class SettingsEditorDialog : public QDialog
{
Q_OBJECT
public:
    explicit SettingsEditorDialog(QWidget *parent = 0);
    ~SettingsEditorDialog();

protected:
    void changeEvent(QEvent *e);

private slots:
    void action(QAbstractButton *button);
    void apply();
    void defaults();
    void save();

private:
    void read();

    Ui::SettingsEditorDialog *ui;

    Settings *_settings;

    QButtonGroup *_buttonGroup;
};

#endif // TANO_SETTINGSEDIT_H_
