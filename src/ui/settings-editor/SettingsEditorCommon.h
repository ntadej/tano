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

#ifndef TANO_SETTINGSPLAYLIST_H_
#define TANO_SETTINGSPLAYLIST_H_

#if defined(Qt5)
    #include <QtWidgets/QWidget>
#elif defined(Qt4)
    #include <QtGui/QWidget>
#endif

namespace Ui
{
    class SettingsEditorCommon;
}

class SettingsEditorCommon : public QWidget
{
Q_OBJECT
public:
    SettingsEditorCommon(QWidget *parent = 0);
    ~SettingsEditorCommon();

    QString radioCategory() const;
    void setRadioCategory(const QString &category);

    bool udpxy() const;
    void setUdpxy(const bool &enabled);
    QString udpxyUrl() const;
    void setUdpxyUrl(const QString &url);
    int udpxyPort() const;
    void setUdpxyPort(const int &port);

    int toolbar() const;
    void setToolbar(const int &id);
    bool splash() const;
    void setSplash(const bool &enabled);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SettingsEditorCommon *ui;
};

#endif // TANO_SETTINGSPLAYLIST_H_
