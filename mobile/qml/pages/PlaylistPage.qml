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
import "../js/core.js" as Tano

Page {
    id: playlistPage
    anchors.margins: UiConstants.DefaultMargin
    tools: commonTools

    function test() {}
    signal channel(string name)

    ListView {
        id: listView
        anchors.fill: parent
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

            Row {
                anchors.fill: parent

                Column {
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        id: mainText
                        text: model.number + " - " + model.name
                        font: UiConstants.TitleFont
                    }

                    Label {
                        id: subText
                        text: model.language + model.categories
                        font: UiConstants.SubtitleFont
                        color: "#cc6633"

                        visible: text != ""
                    }
                }
            }

            /*Image {
                source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
            }*/

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
}
