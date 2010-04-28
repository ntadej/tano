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

TrayIcon::TrayIcon(QMenu *menu) :
	_currentlyRecording(""),
	_currentlyPlaying("")
{
	setContextMenu(menu);
	setIcon(QIcon(":/icons/images/tano.png"));
	setToolTip(tr("Tano"));

	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

TrayIcon::~TrayIcon()
{

}

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
	if (arg.at(0) == "record")
		this->showMessage(tr("Recording"), tr("Tano Recorder is recording %1 to\n%2.").arg(arg.at(1), arg.at(2)), QSystemTrayIcon::Information, 10000);
	else if (arg.at(0) == "latest")
		this->showMessage(tr("Latest version"), tr("You are using the latest version of Tano."), QSystemTrayIcon::Information, 10000);
	else if (arg.at(0) == "svn")
		this->showMessage(tr("SVN"), tr("You are using SVN version:")+" "+arg.at(2)+"\n" + tr("Stable version:") + " " + arg.at(1), QSystemTrayIcon::Information, 10000);
	else if (arg.at(0) == "update")
		this->showMessage(tr("Update available"), tr("A new version of Tano is available!")+"\n" + tr("Version:") + " " + arg.at(1), QSystemTrayIcon::Information, 10000);
}

void TrayIcon::changeToolTip(const QString &text, const QString &type)
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
