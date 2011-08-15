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
import "../data"

import "../js/core.js" as Tano

Page {
    id: homeListPage
    anchors.margins: TanoUi.DefaultMargin

    tools: ToolBarLayoutHome { }

    ListView {
        id: listView
        anchors {left: parent.left; right: parent.right; top: header.bottom; bottom: parent.bottom}
        model: HomeList { }
        pressDelay: 140

        delegate:  Item {
            id: listItem
            height: 88
            width: parent.width

            BorderImage {
                id: background
                anchors.fill: parent
                // Fill page porders
                anchors.leftMargin: -homeListPage.anchors.leftMargin
                anchors.rightMargin: -homeListPage.anchors.rightMargin
                visible: mouseArea.pressed
                source: "image://theme/meegotouch-list-background-pressed-center"
            }

            Row {
                anchors.fill: parent

                Column {
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        id: mainText
                        text: model.title
                        font: TanoUi.TitleFont
                    }

                    Label {
                        id: subText
                        text: model.subtitle
                        font: TanoUi.SubtitleFont
                        color: TanoUi.HighlightColor

                        visible: text != ""
                    }
                }
            }

            Image {
                source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                id: mouseArea
                anchors.fill: background
                onClicked: {
                    Tano.addPage(page)
                }
            }
        }
    }

    PageHeader {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: -homeListPage.anchors.topMargin
        anchors.leftMargin: -homeListPage.anchors.leftMargin
        anchors.rightMargin: -homeListPage.anchors.rightMargin

        title: "Tano Mobile"
    }

    ScrollDecorator {
        flickableItem: listView
    }
}
