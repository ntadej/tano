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
    anchors.margins: TanoUi.DefaultMargin
    tools: ToolBarLayoutPlaylist { }

    function channel(id) {
        Tano.addPage("ScheduleListPage.qml")
        var page = pageStack.currentPage
        page.setChannel(id)
    }

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
                id: icon
                source: "image://theme/icon-m-content-video"
                anchors.left: parent.left;
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 32;
            }

            Label {
                id: numberText
                anchors.left: icon.right;
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 16;
                text: model.number + "."
                font: TanoUi.SubtitleFont
                color: TanoUi.HighlightColor
            }

            Row {
                anchors { left: numberText.right; right: parent.right; top: parent.top; bottom: parent.bottom }
                anchors.leftMargin: 10;

                 Label {
                     id: mainText
                     anchors.verticalCenter: parent.verticalCenter
                     text: model.name
                     font: TanoUi.TitleFont
                     wrapMode: Text.WordWrap
                     width: parent.width
                 }
            }


            MouseArea {
                id: mouseArea
                anchors.fill: background
                onClicked: {
                    playlistPage.channel(epg)
                }
            }
        }
    }
    ScrollDecorator {
        flickableItem: listView
    }

    PageHeader {
        id: playlistHeader
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: -homeListPage.anchors.topMargin
        anchors.leftMargin: -homeListPage.anchors.leftMargin
        anchors.rightMargin: -homeListPage.anchors.rightMargin

        title: qsTr("Select channel")
    }

    Rectangle {
        id: searchBox
        //radius: 10
        color: TanoUi.FieldLabelColor
        anchors {left: parent.left; right: parent.right; top: playlistHeader.bottom}
        anchors.leftMargin: -playlistPage.anchors.leftMargin
        anchors.rightMargin: -playlistPage.anchors.rightMargin
        height: search.height + 2*16

        visible: true

        TextField {
            id: search
            height: 60
            anchors.fill: parent
            anchors.margins: TanoUi.DefaultMargin

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
