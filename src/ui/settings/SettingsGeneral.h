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

#include <QtGui/QWidget>

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

    bool wizard() const;
    void setWizard(const bool &enabled);
    QString language() const;
    void setLanguage(const QString &language);
    bool sessionAutoplay() const;
    void setSessionAutoplay(const bool &enabled);
    bool sessionVolume() const;
    void setSessionVolume(const bool &enabled);

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
