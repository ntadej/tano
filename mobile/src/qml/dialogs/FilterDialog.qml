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

Sheet {
    id: sheet

    property bool populated: false

    function populate() {
        if(sheet.populated)
            return;

        var languages = playlistManager.languages()
        var categories = playlistManager.categories()

        for (var i = 0; i < languages.length; i++) {
            Qt.createQmlObject("import com.meego 1.0; Button { text: \"" + languages[i] + "\"; checkable: true }", buttonsLanguage);
        }

        for (var j = 0; j < categories.length; j++) {
            Qt.createQmlObject("import com.meego 1.0; Button { text: \"" + categories[j] + "\"; checkable: true }", buttonsCategories);
        }

        sheet.populated = true
    }

    acceptButtonText: qsTr("Apply")
    rejectButtonText: qsTr("Reset")

    title: Label {
        id: titleLabel
        anchors.centerIn: parent
        font: TanoUi.TitleFont
        text: qsTr("Filter")
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
                        font: TanoUi.TitleFont
                        text: qsTr("Language:")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    ButtonColumn {
                        id: buttonsLanguage
                        width: screen.currentOrientation == Screen.Landscape ? 300 : 225
                        Button { id: la; text: qsTr("All languages") }
                    }
                }
                Column {
                    Text {
                        id: labelCategories
                        font: TanoUi.TitleFont
                        text: qsTr("Categories:")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    ButtonColumn {
                        id: buttonsCategories
                        width: screen.currentOrientation == Screen.Landscape ? 300 : 225
                        Button { id: ca; text: qsTr("All categories") }
                    }
                }
            }
        }

    }
   onAccepted: {
       playlistManager.processGroups(buttonsCategories.checkedButton.text, buttonsLanguage.checkedButton.text)
   }
   onRejected: {
       la.checked = true
       ca.checked = true
       playlistManager.processGroups(buttonsCategories.checkedButton.text, buttonsLanguage.checkedButton.text)
   }
}
