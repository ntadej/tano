/****************************************************************************
* VlcPlugin.cpp: Recorder Plugin using VLC
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

#include <QtCore/QDateTime>

#include "VlcPlugin.h"

VlcPlugin::VlcPlugin() :
	_output("")
{
#ifdef Q_WS_WIN
	_slash = "\\";
#else
	_slash = "/";
#endif
}

VlcPlugin::~VlcPlugin()
{

}

void VlcPlugin::record(const QString &channelName,
						const QString &channelUrl,
						const QString &recordingDir)
{
	QString fileName = QString(recordingDir);
	fileName.append(_slash);
	fileName.append(QString(channelName).replace(" ","_"));
	fileName.append(QDateTime::currentDateTime().toString("-dd_MM_yyyy-hh_mm_ss"));
	fileName.append(".avi");
#ifdef Q_WS_WIN
	fileName.replace("/","\\");
#endif

	_output = fileName;
}

void VlcPlugin::stop()
{

}

bool VlcPlugin::isRecording() const
{
	//if()
	//	return true;
	//else
		return false;
}
