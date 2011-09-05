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
    id: pageStackWindowPage
    tools: ToolBarLayoutCommon {}
    anchors.margins: UiConstants.DefaultMargin

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: col.width
        contentHeight: col.height
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: col
            spacing: 30   
            width:  flickable.width

            Component.onCompleted: {
                var count = children.length;
                for (var i = 0; i < count; i++) {
                    var item = children[i];
                    item.anchors.horizontalCenter = item.parent.horizontalCenter;
                }
             }

            Button { text: "Toggle StatusBar"; checkable: true; checked: rootWindow.showStatusBar;  onClicked: { rootWindow.showStatusBar = !rootWindow.showStatusBar; } }

            Button { text: "Alternate background image"; checkable: true; checked: rootWindow.platformStyle==customStyle; onClicked: { if (rootWindow.platformStyle==defaultStyle) rootWindow.platformStyle=customStyle; else rootWindow.platformStyle=defaultStyle; } }

            Button { text: "Toggle Rounded corners"; checkable:true; checked: rootWindow.platformStyle.cornersVisible; onClicked: { rootWindow.platformStyle.cornersVisible = !rootWindow.platformStyle.cornersVisible; } }

            Button { text: "Toggle ToolBar"; checkable: true; checked: rootWindow.showToolBar; onClicked: { rootWindow.showToolBar = !rootWindow.showToolBar } }
        }

    }
    ScrollDecorator {
        flickableItem: flickable
    }
}
