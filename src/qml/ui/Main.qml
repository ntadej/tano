import Qt 4.7

import "Core"
import "Playback"

Rectangle {
	id: window

	width: 800; height: 480
	color: "#3E606F"

	FocusScope {
		id: main

		width: parent.width; height: parent.height
		focus: true

		Video {
			id: video;
			width: parent.width; height: parent.height
		}

		Rectangle {
			id: shade
			anchors.fill: parent
			color: "black"
			opacity: 0
		}

		Keys.onDownPressed: osd.focus = true;
	}

	Osd {
		id: osd;
		width: parent.width; height: 100;
		x: 0; y: parent.height;

		Keys.onUpPressed: main.focus = true;
	}

	states: [
		State {
			name: "osdVisible";
			when: osd.activeFocus;
			PropertyChanges { target: osd; y: parent.height-osd.height; }
			PropertyChanges { target: shade; opacity: 0.25 }
		}
	]

	transitions: Transition {
		NumberAnimation { properties: "x,y,opacity"; duration: 600; easing.type: Easing.OutQuint; }
	}
}
