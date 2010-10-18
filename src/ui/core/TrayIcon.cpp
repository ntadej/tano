/****************************************************************************
* TrayIcon.cpp: Tray icon class
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include "TrayIcon.h"

TrayIcon::TrayIcon(QMenu *menu,
				   QObject *parent)
	: QSystemTrayIcon(parent),
	_currentlyRecording(""),
	_currentlyPlaying("")
{
	setContextMenu(menu);
	setIcon(QIcon(":/logo/48x48/logo.png"));
	setToolTip(tr("Tano"));

	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

TrayIcon::~TrayIcon() { }

void TrayIcon::iconActivated(const QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	case QSystemTrayIcon::Trigger:
	case QSystemTrayIcon::DoubleClick:
		emit restoreClick();
		break;
	case QSystemTrayIcon::MiddleClick:
		break;
	default:
		break;
	}
}

void TrayIcon::message(const QStringList &arg)
{
	if (arg[0] == "record")
		this->showMessage(tr("Recording"), tr("Tano Recorder is recording %1 to\n%2.").arg(arg[1], arg[2]), QSystemTrayIcon::Information, 10000);
}

void TrayIcon::changeToolTip(const QString &text,
							 const QString &type)
{
	if(text != "stop" && type == "main") {
		_currentlyPlaying = text;
	} else if(text != "stop" && type == "recorder") {
		_currentlyRecording = text;
	} else if(text == "stop" && type == "main") {
		_currentlyPlaying = "";
	} else if(text == "stop" && type == "recorder") {
		_currentlyRecording = "";
	}

	this->setToolTip(tr("Tano"));
	if(_currentlyPlaying != "")
		this->setToolTip(this->toolTip().append(" - " + tr("Playing:") + " " + _currentlyPlaying));
	if(_currentlyRecording != "")
			this->setToolTip(this->toolTip().append(" - " + tr("Recording:") + " " + _currentlyRecording));
}
