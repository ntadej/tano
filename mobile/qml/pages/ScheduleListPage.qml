/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
*
* This file was based on the examples of the Qt Toolkit.
* Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
* Contact: Qt Software Information (qt-info@nokia.com)
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

import "../common"

import "../js/core.js" as Tano

Page {
    id: scheduleListPage
    anchors.margins: UiConstants.DefaultMargin
    tools: ToolBarLayoutCommon { }

    function item(name) {
        //Tano.addPage("SchedulePage.qml")
        //var page = pageStack.currentPage
        //page.setChannel(name)
    }

    function setChannel(epg) {
        for (var i = 0; i < buttonsDate.children.length; i++) {
            buttonsDate.children[i].destroy()
        }

        xmltvManager.openXmltv(epg)

        var dates = xmltvManager.dates()
        for (var j = 0; j < dates.length; j++) {
            Qt.createQmlObject("import com.meego 1.0; Button { text: \"" + dates[j] + "\"; checkable: true }", buttonsDate);
        }
    }

    ListView {
        id: listView
        anchors {left: parent.left; right: parent.right; top: dateBox.bottom; bottom: parent.bottom}
        model: xmltvModel
        pressDelay: 140

        delegate:  Item {
            id: listItem
            height: 88
            width: parent.width

            BorderImage {
                id: background
                anchors.fill: parent
                // Fill page porders
                anchors.leftMargin: -scheduleListPage.anchors.leftMargin
                anchors.rightMargin: -scheduleListPage.anchors.rightMargin
                visible: mouseArea.pressed
                source: "image://theme/meegotouch-list-background-pressed-center"
            }

            Image {
                id: icon
                source: "image://theme/icon-m-content-event"
                anchors.left: parent.left;
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                id: timeText
                anchors.left: icon.right;
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 16;
                text: Qt.formatDateTime(model.start, "HH:mm")
                font: UiConstants.SubtitleFont
                color: "#cc6633"
            }

            Row {
                anchors { left: timeText.right; right: parent.right; top: parent.top; bottom: parent.bottom }
                anchors.leftMargin: 10;

                 Label {
                     id: mainText
                     anchors.verticalCenter: parent.verticalCenter
                     text: model.title
                     font: UiConstants.TitleFont
                     wrapMode: Text.WordWrap
                     width: parent.width
                 }
            }

            MouseArea {
                id: mouseArea
                anchors.fill: background
                onClicked: {
                    scheduleListPage.item(start)
                }
            }
        }
    }
    ScrollDecorator {
        flickableItem: listView
    }

    Rectangle {
        id: dateBox
        radius: 10
        color: "lightgrey"
        anchors {left: parent.left; right: parent.right; top: parent.top}
        anchors.leftMargin: -scheduleListPage.anchors.leftMargin
        anchors.rightMargin: -scheduleListPage.anchors.rightMargin
        anchors.topMargin: -scheduleListPage.anchors.topMargin
        anchors.bottomMargin: -scheduleListPage.anchors.bottomMargin
        height: 50 + 2*16

        visible: true

        ButtonRow {
            id: buttonsDate

            anchors.fill: parent
            anchors.margins: UiConstants.DefaultMargin

            onCheckedButtonChanged: xmltvManager.processDate(buttonsDate.checkedButton.text)
        }
    }
}
