/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_RECORDERDBUSADAPTOR_H_
#define TANO_RECORDERDBUSADAPTOR_H_

#include <QtCore/QObject>
#include <QtDBus/QtDBus>

#include "recorder/RecorderMain.h"

class RecorderDBusAdaptor : public QDBusAbstractAdaptor
{
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "si.tano.TanoPlayer.Recorder")

public:
	RecorderDBusAdaptor(RecorderMain *recorder);
	~RecorderDBusAdaptor();

public slots:
	QString output() const { return _main->output(); }
	void record(const QString &channel,
				const QString &url,
				const QString &path) { _main->record(channel, url, path); }
	bool recording() const { return _main->isRecording(); }
	void refreshTimers() { _main->refreshTimers(); }
	void stop() { _main->stop(); }
	bool timer() const { return _main->isTimer(); }

signals:
	void elapsed(const int &);
	void stopTimer();
	void timer(const QString &,
			   const QString &,
			   const QString &);

private:
	RecorderMain *_main;
};

#endif // TANO_RECORDERDBUSADAPTOR_H_
