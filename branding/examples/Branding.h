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

#ifndef TANO_BRANDING_H_
#define TANO_BRANDING_H_

#include <QtCore/QObject>

#include "WidgetsSharedExport.h"

class MainWindow;

namespace Ui
{
    class MainWindow;
    class SettingsDialog;
}

class QMenu;

namespace Tano
{
    namespace Branding
    {
        TANO_WIDGETS_EXPORT void processMenus(MainWindow *main,
                                              Ui::MainWindow *ui,
                                              QMenu *right);
        TANO_WIDGETS_EXPORT void processSettings(Ui::SettingsDialog *ui);
    }
}

#endif // TANO_BRANDING_H_
