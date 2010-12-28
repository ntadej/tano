/****************************************************************************
* FripTV - Tano Recorder Plugin
* Copyright (C) 2010 Tadej Novak <tadej@tano.si>
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

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>

#include "FripTv.h"

FripTv::FripTv()
	: _fripProcess(new QProcess()),
	_fripPath(fripPath()) { }

FripTv::~FripTv() { }

void FripTv::record(const QString &channelName,
					const QString &channelUrl,
					const QString &file)
{
	QFile f(QDir::tempPath()+"/tano.txt");
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&f);
	out << "#EXTM3U" << "\n"
		<< "#EXTINF:0," << channelName << "\n"
		<< channelUrl;

	QString fileName = file;
#ifdef Q_WS_WIN
	fileName.replace("/","\\");
#endif

	QStringList arguments;
	arguments << "-cl";

#ifdef Q_WS_WIN
	arguments << QString(QDir::tempPath() + "/tano.txt").replace("/","\\");
#else
	arguments << QString(QDir::tempPath() + "/tano.txt");
#endif
	arguments << "-s"
			  << "-fi"
			  << fileName;

	_fripProcess->start(_fripPath, arguments);
}

void FripTv::stop()
{
	_fripProcess->kill();
}

bool FripTv::isRecording() const
{
	if(_fripProcess->state() == QProcess::Running || _fripProcess->state() == QProcess::Starting)
		return true;
	else
		return false;
}

bool FripTv::fripExists() const
{
	if (QFileInfo(fripPath()).exists())
		return true;
	else
		return false;
}

QString FripTv::fripPath() const
{
#ifdef Q_WS_X11
	return "friptv";
#else
	return QCoreApplication::applicationDirPath() + "/friptv/friptv.exe";
#endif
}
