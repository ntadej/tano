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

import QtQuick 1.0
import com.meego 1.0

ToolBarLayout {
    id: toolBarLayoutCommon
    visible: false
    ToolIcon {
        iconId: "toolbar-back"
        onClicked: {
            mainMenu.close();
            pageStack.pop();
        }
    }
    ToolButton {
        text: "Change orientation";
        onClicked: orientationLock = (screen.currentOrientation == Screen.Portrait) ?
                       PageOrientation.LockLandscape : PageOrientation.LockPortrait
    }
    ToolIcon {
        iconId: "toolbar-filter"
        onClicked: {
            filter.populate()
            filter.open()
        }
    }
    ToolIcon {
        iconId: "toolbar-view-menu"
        onClicked: (mainMenu.status == DialogStatus.Closed) ? mainMenu.open() : mainMenu.close()
    }
}
