/*
 * MPlayer backend for the Phonon library
 * Copyright (C) 2006-2008  Ricardo Villalba <rvm@escomposlinux.org>
 * Copyright (C) 2007-2008  Tanguy Krotoff <tkrotoff@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MyProcess.h"

#include <QtCore/QtDebug>

MyProcess::MyProcess(QObject * parent)
	: QProcess(parent) {

	//QProcess::MergedChannels
	//QProcess merges the output of the running process into the standard
	//output channel (stdout). The standard error channel (stderr) will not
	//receive any data. The standard output and standard error data of the
	//running process are interleaved.
	setProcessChannelMode(QProcess::MergedChannels);

	//QProcess::readyReadStandardOutput()
	//This signal is emitted when the process has made new data available through
	//its standard output channel (stdout). It is emitted regardless of the
	//current read channel.
	connect(this, SIGNAL(readyReadStandardOutput()), SLOT(readStdout()));

	connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(finished(int, QProcess::ExitStatus)));

	connect(this, SIGNAL(error(QProcess::ProcessError)), SLOT(error(QProcess::ProcessError)));
}

MyProcess::~MyProcess() {
}

bool MyProcess::isRunning() const {
	return (state() == QProcess::Running);
}

void MyProcess::start(const QString & program, const QStringList & arguments) {
	_remainingOutput.clear();

	qDebug() << __FUNCTION__ << "Process started:" << program << arguments.join(" ");
	QProcess::start(program, arguments);
}

void MyProcess::readStdout() {
	genericRead(readAllStandardOutput());
}

void MyProcess::genericRead(const QByteArray & output) {
	QByteArray totalOutput = _remainingOutput + output;
	int start = 0;
	int from = 0;
	int pos = canReadLine2(totalOutput, from);

	while (pos > -1) {
		//Readline
		//QString line = totalOutput.mid(start, pos - start);
		//cp1252
		//Windows-1252
		//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1252"));
		QString line = QString::fromLocal8Bit(totalOutput.mid(start, pos - start));
		from = pos + 1;

#ifdef Q_OS_WIN
		if ((from < totalOutput.size()) && (totalOutput.at(from) == '\n')) {
			from++;
		}
#endif	//Q_OS_WIN

		start = from;

		//QString::trimmed() is used for removing leading and trailing whitespaces
		//Some .mp3 files contain tags with starting and ending whitespaces
		//Unfortunately MPlayer gives us leading and trailing whitespaces,
		//Winamp for example doesn't show them
		line = line.trimmed();
		if (!line.isEmpty()) {
			emit lineAvailable(line);
		}

		pos = canReadLine2(totalOutput, from);
	}

	_remainingOutput = totalOutput.mid(from);
}

int MyProcess::canReadLine2(const QByteArray & output, int from) {
	int pos1 = output.indexOf('\n', from);
	int pos2 = output.indexOf('\r', from);

	if ((pos1 == -1) && (pos2 == -1)) {
		return -1;
	}

	int pos = pos1;
	if ((pos1 != -1) && (pos2 != -1)) {
		if (pos1 < pos2) {
			pos = pos1;
		} else {
			pos = pos2;
		}
	} else {
		if (pos1 == -1) {
			pos = pos2;
		} else if (pos2 == -1) {
			pos = pos1;
		}
	}

	return pos;
}

void MyProcess::finished(int exitCode, QProcess::ExitStatus exitStatus) {
	qDebug() << __FUNCTION__ << "Bytes still available:" << bytesAvailable();

	if (bytesAvailable() > 0) {
		readStdout();
	}

	qDebug() << __FUNCTION__ << "Process ended: exitCode:" << exitCode << "exitStatus:" << exitStatus;
}

void MyProcess::error(QProcess::ProcessError error) {
	switch (error) {
	case QProcess::FailedToStart:
		qCritical() << __FUNCTION__ << "Error: QProcess::FailedToStart";
		break;
	case QProcess::Crashed:
		qCritical() << __FUNCTION__ << "Error: QProcess::Crashed";
		break;
	case QProcess::Timedout:
		qCritical() << __FUNCTION__ << "Error: QProcess::Timedout";
		break;
	case QProcess::WriteError:
		qCritical() << __FUNCTION__ << "Error: QProcess::WriteError";
		break;
	case QProcess::ReadError:
		qCritical() << __FUNCTION__ << "Error: QProcess::ReadError";
		break;
	case QProcess::UnknownError:
		qCritical() << __FUNCTION__ << "Error: QProcess::UnknownError";
		break;
	default:
		qCritical() << __FUNCTION__ << "Error: unknown QProcess::ProcessError:" << error;
	}
}
