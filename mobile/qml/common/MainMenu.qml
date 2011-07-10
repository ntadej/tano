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

import "../dialogs"

Menu {
    id: menu

    AboutDialog { id: about }
    MenuLayout {
        MenuItem { text: qsTr("Settings"); onClicked: Tano.addPage("pages/SettingsPage.qml") }
        MenuItem { text: qsTr("Help"); onClicked: Tano.addPage("pages/HelpPage.qml") }
        MenuItem { text: qsTr("About"); onClicked: about.open() }
        MenuItem { text: qsTr("Exit"); onClicked: Qt.quit() }
    }
}
