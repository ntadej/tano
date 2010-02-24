/****************************************************************************
* Recorder.h: Class for recording management
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

#ifndef TANO_RECORDER_H_
#define TANO_RECORDER_H_

#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtGui/QMainWindow>

#include "Timer.h"
#include "../plugins/RecorderPlugins.h"
#include "../ui/TrayIcon.h"
#include "../xml/M3UHandler.h"

#include <ui_Recorder.h>

class Recorder : public QWidget
{
Q_OBJECT
public:
	Recorder(QWidget *parent = 0);
	~Recorder();

	bool isRecording() const {return _recording;};
	void setGlobals(TrayIcon *icon, QAction *action);

public slots:
	void stop();
	void recordNow(const QString &name, const QString &url);
	void openPlaylist(const QString &file);
	void recordTimer(Timer *timer);
	void stopTimer(Timer *timer);

private slots:
	void record(const bool &status);

	void sec();

	void playlist(QTreeWidgetItem* clickedChannel);
	void fileBrowse();

private:
	Ui::Recorder ui;

	bool _recording;
	bool _isTimer;

	QString _channelName;
	QString _channelUrl;

	RecorderPlugin *_plugin;
	TrayIcon *_trayIcon;
	Timer *_currentTimer;

	QTimer *_timer;
	QTime _time;
	QAction *_actionRecord;
};

#endif // TANO_RECORDER_H_
