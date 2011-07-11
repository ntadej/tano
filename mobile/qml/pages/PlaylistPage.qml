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
import "../dialogs"

import "../js/core.js" as Tano

Page {
    id: playlistPage
    anchors.margins: UiConstants.DefaultMargin
    tools: ToolBarLayoutPlaylist { }

    signal channel(string name)

    FilterDialog { id: filter }

    ListView {
        id: listView
        anchors {left: parent.left; right: parent.right; top: searchBox.bottom; bottom: parent.bottom}
        model: channelsModel
        pressDelay: 140

        delegate:  Item {
            id: listItem
            height: 88
            width: parent.width

            visible: model.hidden !== true

            BorderImage {
                id: background
                anchors.fill: parent
                // Fill page porders
                anchors.leftMargin: -playlistPage.anchors.leftMargin
                anchors.rightMargin: -playlistPage.anchors.rightMargin
                visible: mouseArea.pressed
                source: "image://theme/meegotouch-list-background-pressed-center"
            }

            Image {
                source: "image://theme/icon-m-content-video"
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 32;
            }

            Row {
                anchors.fill: parent

                Column {
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        id: mainText
                        text: model.number + ". " + model.name
                        font: UiConstants.TitleFont
                    }
                }
            }

            MouseArea {
                id: mouseArea
                anchors.fill: background
                onClicked: {
                    playlistPage.channel(url)
                    pageStack.pop()
                }
            }
        }
    }
    ScrollDecorator {
        flickableItem: listView
    }

    Rectangle {
        id: searchBox
        radius: 10
        color: "lightgrey"
        anchors {left: parent.left; right: parent.right; top: parent.top}
        anchors.leftMargin: -playlistPage.anchors.leftMargin
        anchors.rightMargin: -playlistPage.anchors.rightMargin
        anchors.topMargin: -playlistPage.anchors.topMargin
        anchors.bottomMargin: -playlistPage.anchors.bottomMargin
        height: 72

        visible: true

        TextField {
            id: search
            height: 40
            anchors.fill: parent
            anchors.margins: UiConstants.DefaultMargin

            platformSipAttributes: SipAttributes { actionKeyHighlighted: true }

            placeholderText: qsTr("Search ...")
            platformStyle: TextFieldStyle { paddingRight: clearButton.width }
            Image {
                id: clearButton
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                source: "image://theme/icon-m-input-clear"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        search.text = "";
                    }
                }
            }

            onTextChanged: playlistManager.processSearch(search.text)
        }
    }
}
