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

#ifndef TANO_SETTINGSSCHEDULE_H_
#define TANO_SETTINGSSCHEDULE_H_

#include <QtGui/QWidget>

struct XmltvGrabber;
class XmltvSystem;

namespace Ui
{
    class SettingsSchedule;
}

class SettingsSchedule : public QWidget
{
Q_OBJECT
public:
    SettingsSchedule(QWidget *parent = 0);
    ~SettingsSchedule();

    QString location() const;
    void setLocation(const QString &location);
    bool update() const;
    void setUpdate(const bool &enabled);
    bool updateGrabber() const;
    void setUpdateGrabber(const bool &enabled);
    QString updateUrl() const;
    void setUpdateUrl(const QString &url);
    QString grabber() const;
    QString grabberPath() const;
    void setGrabber(const QString &grabber);

protected:
    void changeEvent(QEvent *e);

private slots:
    void listGrabbers(const QList<XmltvGrabber> &list);

private:
    Ui::SettingsSchedule *ui;

    QString _grabber;
    XmltvSystem *_xmltv;
};

#endif // TANO_SETTINGSSCHEDULE_H_
