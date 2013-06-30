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

#ifndef TANO_SETTINGSUI_H_
#define TANO_SETTINGSUI_H_

#if defined(Qt5)
    #include <QtWidgets/QWidget>
#elif defined(Qt4)
    #include <QtGui/QWidget>
#endif

namespace Ui
{
    class SettingsUi;
}

class SettingsUi : public QWidget
{
Q_OBJECT
public:
    SettingsUi(QWidget *parent = 0);
    ~SettingsUi();

    bool osd() const;
    void setOsd(bool enabled);
    bool info() const;
    void setInfo(bool enabled);
    bool tray() const;
    void setTray(bool enabled);
    bool trayHide() const;
    void setTrayHide(bool enabled);
    QString wheel() const;
    void setWheel(const QString &wheel);
    int toolbar() const;
    void setToolbar(int id);
    bool rememberSize() const;
    void setRememberSize(bool enabled);
    bool filter() const;
    void setFilter(bool enabled);

    QString icons() const;
    void setIcons(const QString &icons);

    bool lite() const;
    void setLite(bool enabled);
    bool top() const;
    void setTop(bool enabled);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SettingsUi *ui;
};

#endif // TANO_SETTINGSUI_H_
