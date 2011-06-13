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

#ifndef TANO_MENUCORE_H_
#define TANO_MENUCORE_H_

#include <QtGui/QMenu>

#include <vlc-qt/Enums.h>

class MenuCore : public QMenu
{
Q_OBJECT
public:
    MenuCore(QWidget *parent = 0);
    ~MenuCore();

    QAction *actionNext() { return _next; }
    void addItem(QAction *action);
    void setType(const Vlc::ActionsType &type) { _type = type; }
    Vlc::ActionsType type() const { return _type; }

public slots:
    void setActions(const QList<QAction*> &actions,
                    const Vlc::ActionsType &type);

private slots:
    void next();

private:
    QActionGroup *_group;
    QAction *_next;

    Vlc::ActionsType _type;
};

#endif // TANO_MENUCORE_H_
