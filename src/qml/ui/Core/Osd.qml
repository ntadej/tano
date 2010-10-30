/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

import Qt 4.7

FocusScope {
	id: container
	width: parent.width; height: parent.height

	Item {
		anchors.fill: parent
		width: parent.width; height: parent.height
		focus: false

		Rectangle {
			width: parent.width; height: parent.height
			gradient: Gradient {
				GradientStop { position: 0.00; color: "#a01010"; }
				GradientStop { position: 1.00; color: Qt.darker("#ffffff"); }
			}
		}
	}
}
