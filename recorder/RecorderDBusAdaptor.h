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
Q_PROPERTY(bool recording READ recording)
Q_PROPERTY(bool timer READ timer)
Q_PROPERTY(QString output READ output)

public:
	RecorderDBusAdaptor(RecorderMain *recorder);
	~RecorderDBusAdaptor();

	bool recording() const { return _main->isRecording(); }
	bool timer() const { return _main->isTimer(); }
	QString output() const { return _main->output(); }

public slots:
	void record(const QString &channel,
				const QString &url,
				const QString &path) { _main->record(channel, url, path); }
	void refreshBackend() { _main->refreshBackend(); }
	void refreshTimers() { _main->refreshTimers(); }
	void stop() { _main->stop(); }

signals:
	void elapsed(const int &);

private:
	RecorderMain *_main;
};

#endif // TANO_RECORDERDBUSADAPTOR_H_
