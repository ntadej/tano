import QtQuick 1.0
import com.meego 1.0

Sheet {
    id: sheet

    function populate(languages, categories) {
        for (var i = 0; i < languages.length; i++) {
            Qt.createQmlObject("import com.meego 1.0; Button { text: \"" + languages[i] + "\"; checkable: true }", buttonsLanguage);
        }

        for (var j = 0; j < categories.length; j++) {
            Qt.createQmlObject("import com.meego 1.0; Button { text: \"" + categories[j] + "\"; checkable: true }", buttonsCategories);
        }
    }

    acceptButtonText: "Apply"
    rejectButtonText: "Cancel"

    title: Label {
        id: titleLabel
        anchors.centerIn: parent
        font: UiConstants.TitleFont
        text: qsTr("Filter playlist")
    }

    content: Flickable {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.topMargin: 10
        contentWidth: parent.width
        contentHeight: col.height
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: col
            anchors.horizontalCenter: parent.horizontalCenter

            Row {
                Column {
                    Text {
                        id: labelLanguage
                        font.pixelSize: 22
                        text: qsTr("Language:")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    ButtonColumn {
                        id: buttonsLanguage
                        Button { text: qsTr("All") }
                    }
                }
                Column {
                    Text {
                        id: labelCategories
                        font.pixelSize: 22
                        text: qsTr("Categories:")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    ButtonColumn {
                        id: buttonsCategories
                        Button { text: qsTr("All") }
                    }
                }
            }
        }

    }
   // onAccepted: label.text = "Accepted!"
   // onRejected: label.text = "Rejected!"
}
