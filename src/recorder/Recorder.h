/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <info@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef TANO_RECORDER_H_
#define TANO_RECORDER_H_

#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtGui/QWidget>

#include "container/Timer.h"
#include "plugins/RecorderPlugins.h"
#include "recorder/Time.h"
#include "ui/recorder/TimersEdit.h"

namespace Ui
{
	class Recorder;
}

class Recorder : public QWidget
{
Q_OBJECT
public:
	Recorder(QWidget *parent = 0);
	~Recorder();

	void createSettings();
	bool isRecording() const { return _recording; }
	void setAction(QAction *action);

protected:
	void changeEvent(QEvent *e);

public slots:
	void openPlaylist(const QString &file);
	void recordNow(const QString &name,
				   const QString &url);
	void recordTimer(Timer *timer);
	void showTimersEditor();
	void stop();
	void stopTimer(Timer *timer);

private slots:
	void fileBrowse();
	void playlist(QTreeWidgetItem* clickedChannel);
	void record(const bool &status);
	void sec();

private:
	Ui::Recorder *ui;

	bool _recording;
	bool _isTimer;

	QString _channelName;
	QString _channelUrl;

	RecorderPlugin *_plugin;
	Time *_timeManager;
	Timer *_currentTimer;
	TimersEdit *_editor;

	QTimer *_timer;
	QTime _time;
	QAction *_actionRecord;
};

#endif // TANO_RECORDER_H_
