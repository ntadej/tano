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

#ifndef TANO_TANOAPPLICATION_H_
#define TANO_TANOAPPLICATION_H_

#include "application/SingleApplication.h"

#include "WidgetsSharedExport.h"

class Arguments;

class TANO_WIDGETS_EXPORT TanoApplication : public SingleApplication
{
Q_OBJECT
public:
    explicit TanoApplication(int argc,
                             char *argv[]);
    ~TanoApplication();

    inline Arguments *arguments() { return _arguments; }
    static bool preInit();
    bool postInit();

#ifdef Q_OS_MAC
    void setupDockHandler();
#endif

public slots:
    void onClickOnDock();

signals:
    void dockClicked();

private:
    Arguments *_arguments;
};

#endif // TANO_TANOAPPLICATION_H_
