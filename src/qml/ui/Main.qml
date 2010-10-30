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

import "Core"
import "Playback"

Rectangle {
	id: window

	width: 800; height: 600
	color: "#3E606F"

	FocusScope {
		id: main

		width: parent.width; height: parent.height
		focus: true

		Video {
			id: video
			width: parent.width; height: parent.height
		}

		Rectangle {
			id: shade
			anchors.fill: parent
			color: "black"
			opacity: 0
		}

		Keys.onDownPressed: osd.focus = true
	}

	Osd {
		id: osd
		width: parent.width; height: 100
		x: 0; y: parent.height

		Keys.onUpPressed: main.focus = true
	}

	states: [
		State {
			name: "osdVisible"
			when: osd.activeFocus
			PropertyChanges { target: osd; y: parent.height-osd.height; }
			PropertyChanges { target: shade; opacity: 0.25 }
		}
	]

	transitions: Transition {
		NumberAnimation { properties: "x,y,opacity"; duration: 600; easing.type: Easing.OutQuint; }
	}
}
