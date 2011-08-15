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
    id: helpPage
    anchors.margins: TanoUi.DefaultMargin
    tools: ToolBarLayoutCommon {}
    
    Flickable {        
        contentWidth: childrenRect.width
        contentHeight: childrenRect.height
        flickableDirection: Flickable.VerticalFlick

        anchors.top: helpHeader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Column {
            Label { text: "Help page" }
        }
    }

    PageHeader {
        id: helpHeader
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: -helpPage.anchors.topMargin
        anchors.leftMargin: -helpPage.anchors.leftMargin
        anchors.rightMargin: -helpPage.anchors.rightMargin

        title: "Help"
    }
}
