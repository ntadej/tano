import Qt 4.7

FocusScope {
	id: container;

	Item {
		anchors.fill: parent;
		focus: false;

		Rectangle {
			width: parent.width; height: parent.height;
			gradient: Gradient {
				GradientStop {
					position: 0.00;
					color: "#a01010";
				}
				GradientStop {
					position: 1.00;
					color: Qt.darker("#ffffff");
				}
			}
		}
	}
}
