import Qt 4.7
import VideoDisplay 0.1

FocusScope {
	id: container;

	Item {
		anchors.fill: parent;
		VideoDisplay {
			anchors.fill: parent;
			width: parent.width; height: parent.height;

			Keys.onDownPressed: osd.focus = true;
		}
	}
}
