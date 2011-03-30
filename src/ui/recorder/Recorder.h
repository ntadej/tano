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

#ifndef TANO_RECORDER_H_
#define TANO_RECORDER_H_

#include <QtCore/QTime>
#include <QtGui/QWidget>
#include <QtGui/QTreeWidgetItem>

class RecorderController;
class RecorderProcess;
class TrayIcon;
class TimersEdit;

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
	bool isRecording() const;
	void setAction(QAction *action);
	void setTrayIcon(TrayIcon *icon);

protected:
	void changeEvent(QEvent *e);

public slots:
	void openPlaylist(const QString &file);
	void recordNow(const QString &name,
				   const QString &url);
	void showTimersEditor();
	void stop();

private slots:
	void fileBrowse();
	void playlist(QTreeWidgetItem* clickedChannel);
	void record(const bool &status);
	void time(const int &time);
	void timerStart(const QString &name,
					const QString &url);
	void timerStop();

private:
	Ui::Recorder *ui;

	QAction *_actionRecord;

	QString _name;
	QString _url;

	RecorderController *_controller;
	TimersEdit *_editor;
	QString _playlist;
	RecorderProcess *_recorder;
	TrayIcon *_trayIcon;
};

#endif // TANO_RECORDER_H_
