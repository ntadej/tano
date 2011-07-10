import QtQuick 1.0
import com.meego 1.0

Dialog {
    id: aboutDialog
    title: Item {
        id: titleField
        height: aboutDialog.platformStyle.titleBarHeight
        width: parent.width
        Image {
            id: supplement
            source: "image://theme/icon-l-contacts"
            height: parent.height - 10
            width: 75
            fillMode: Image.PreserveAspectFit
            anchors.leftMargin: 5
            anchors.rightMargin: 5
        }

        Label {
            id: titleLabel
            anchors.left: supplement.right
            anchors.verticalCenter: titleField.verticalCenter
            font.capitalization: Font.MixedCase
            color: "white"
            text: qsTr("About Tano Mobile")
        }

        Image {
            id: closeButton
            anchors.verticalCenter: titleField.verticalCenter
            anchors.right: titleField.right

            source: "image://theme/icon-m-framework-close"

            MouseArea {
                id: closeButtonArea
                anchors.fill: parent
                onClicked:  { aboutDialog.reject(); }
            }
        }
    }

    content:Item {
        id: name
        height: childrenRect.height
        Text {
            id: text
            font.pixelSize: 22
            color: "white"
            text: qsTr("Tano is an open-source cross-platform IP TV player which features\nfull IP TV playback with EPG and recorder.\n") +
                  qsTr("It is based on VLC-Qt.\n\n") +
                  qsTr("You are using the mobile version which is currently in\nan experimental stage.")
        }
    }

    buttons: Button {
        platformStyle: ButtonStyle { }
        id: b1; text: qsTr("Close"); onClicked: aboutDialog.accept()
    }
}
