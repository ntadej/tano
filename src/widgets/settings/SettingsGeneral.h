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

#ifndef TANO_SETTINGSGENERAL_H_
#define TANO_SETTINGSGENERAL_H_

#if defined(Qt5)
    #include <QtWidgets/QWidget>
#elif defined(Qt4)
    #include <QtGui/QWidget>
#endif

namespace Ui
{
    class SettingsGeneral;
}

class SettingsGeneral : public QWidget
{
Q_OBJECT
public:
    SettingsGeneral(QWidget *parent = 0);
    ~SettingsGeneral();

    QString language() const;
    void setLanguage(const QString &language);
    bool sessionAutoplay() const;
    void setSessionAutoplay(bool enabled);
    bool sessionVolume() const;
    void setSessionVolume(bool enabled);

protected:
    void changeEvent(QEvent *e);

signals:
    void resetDefaults();

private:
    Ui::SettingsGeneral *ui;

    void loadLocale();
    QStringList _locale;
};

#endif // TANO_SETTINGSGENERAL_H_
