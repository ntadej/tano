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

QueryDialog {
    titleText: qsTr("About Tano Mobile")
    message: qsTr("Tano is an open-source cross-platform IP TV player which features full IP TV playback with EPG and recorder.\n") +
             qsTr("It is based on VLC-Qt.\n\n") +
             qsTr("You are using the mobile version which is currently in an experimental stage.")
    rejectButtonText: "Close"
}
