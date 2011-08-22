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

ListModel {
    id: homePagesModel
    ListElement {
        type: "playback"
        page: "PlaylistPage.qml"
        title: "Play live TV"
        subtitle: ""
    }
    ListElement {
        type: "schedule"
        page: "PlaylistPage.qml"
        title: "Schedule"
        subtitle: "Description"
    }
    ListElement {
        type: "test"
        page: "../test/TestListPage.qml"
        title: "Tests"
        subtitle: "Description"
    }
}
