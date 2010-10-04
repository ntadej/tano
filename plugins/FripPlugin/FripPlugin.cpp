/****************************************************************************
* FripPlugin.cpp: Recorder Plugin using friptv
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

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>

#include "FripPlugin.h"

FripPlugin::FripPlugin()
	: _fripProcess(new QProcess()),
	_fripPath(fripPath()),
	_output("")
{
#ifdef Q_WS_WIN
	_slash = "\\";
#else
	_slash = "/";
#endif
}

FripPlugin::~FripPlugin() { }

void FripPlugin::record(const QString &channelName,
						const QString &channelUrl,
						const QString &recordingDir)
{
	QFile file(QDir::tempPath()+"/tano.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);
	out << "#EXTM3U" << "\n"
		<< "#EXTINF:0," << channelName << "\n"
		<< channelUrl;

	QString fileName = QString(recordingDir);
	fileName.append(_slash);
	fileName.append(QString(channelName).replace(" ","_"));
	fileName.append(QDateTime::currentDateTime().toString("-dd_MM_yyyy-hh_mm_ss"));
	fileName.append(".avi");
#ifdef Q_WS_WIN
	fileName.replace("/","\\");
#endif

	QStringList arguments;
	arguments << "-cl";

#ifdef Q_WS_WIN
	arguments << QString(QDir::tempPath()+_slash+"tano.txt").replace("/","\\");
#else
	arguments << QString(QDir::tempPath()+_slash+"tano.txt");
#endif
	arguments << "-s"
			  << "-fi"
			  << fileName;

	_output = fileName;
	_fripProcess->start(_fripPath, arguments);
}

void FripPlugin::stop()
{
	_fripProcess->kill();
}

bool FripPlugin::isRecording() const
{
	if(_fripProcess->state() == QProcess::Running || _fripProcess->state() == QProcess::Starting)
		return true;
	else
		return false;
}

bool FripPlugin::fripExists() const
{
	if (QFileInfo(fripPath()).exists())
		return true;
	else
		return false;
}

QString FripPlugin::fripPath() const
{
#ifdef Q_WS_X11
	return "friptv";
#else
	return QCoreApplication::applicationDirPath() + "/friptv/friptv.exe";
#endif
}
