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

Page {
    id: visibilityPage
    anchors.margins: UiConstants.DefaultMargin
    tools: ToolBarLayoutCommon {}

    function updateViewMode() {
        if (platformWindow.viewMode == WindowState.Fullsize) {
            l1.item.color = "green";
        } else {
            l1.item.color = "red";
        }

        l1.item.text = platformWindow.viewModeString;
    }

    function updateVisible() {
        if (platformWindow.visible) {
            l2.item.color = "green";
            l2.item.text = "visible";
        } else {
            l2.item.color = "red";
            l2.item.text = "invisible";
        }
    }

    function updateActive() {
        if (platformWindow.active) {
            l3.item.color = "green";
            l3.item.text = "active";
        } else {
            l3.item.color = "red";
            l3.item.text = "inactive";
        }
    }

    Connections {
        target: platformWindow

        onViewModeChanged: updateViewMode()
        onVisibleChanged: updateVisible()
        onActiveChanged: updateActive()
    }

    Component {
        id: textBox

        Rectangle {
            property alias text: textItem.text

            width: 200; height: 150
            color: "yellow"
            border.color: "black"
            border.width: 5
            radius: 10

            Text {
                id: textItem
                anchors.centerIn: parent
                font.pointSize: 32
                color: "black"
            }
        }
    }
    
    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: col.width
        contentHeight: col.height
        flickableDirection: Flickable.VerticalFlick    

        Column {
            id: col
            spacing: 10
            width: flickable.width
            
            Loader {
                id: l1
                sourceComponent: textBox
            }
    
            Loader {
                id: l2
                sourceComponent: textBox
            }
    
            Loader {
                id: l3
                sourceComponent: textBox
            }
    
            Component.onCompleted: {
                updateViewMode();
                updateVisible();
                updateActive();
                
                var count = children.length;
                for (var i = 0; i < count; i++) {
                    var item = children[i];
                    item.anchors.horizontalCenter = item.parent.horizontalCenter;
                }                
            }
        }
    }
}
