/****************************************************************************
* EditTimers.h: Timers editor and manager
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

#ifndef TANO_EDITTIMERS_H_
#define TANO_EDITTIMERS_H_

#include <QtCore/QMap>
#include <QtCore/QSettings>
#include <QtGui/QMainWindow>
#include <QtGui/QTreeWidgetItem>

#include "channels/Channel.h"
#include "control/Time.h"
#include "recorder/Timer.h"
#include "xml/TimersHandler.h"

#include <ui_EditTimers.h>

class EditTimers : public QMainWindow
{
Q_OBJECT
public:
	EditTimers(Time *t, const QString &playlist, QWidget *parent = 0);
	~EditTimers();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void newItem();
	void addItem();
	void deleteItem();

	void edit(QTreeWidgetItem *item);
	void playlist(QTreeWidgetItem *item);

	void validate();
	void changeStatus(Timer* t, const bool &status);

	void editName(const QString &name);
	void editDate(const QDate &date);
	void editStartTime(const QTime &time);
	void editEndTime(const QTime &time);

	void read(const QString &file = 0);
	void write();
	void exit();

private:
	void createConnections();
	void createSettings();

	Ui::EditTimers ui;

	bool _activeTimers;
	bool _closeEnabled;

	QString _playlist;

	Time *_time;
	TimersHandler *_handler;

	Channel *_channel;
};

#endif // TANO_EDITTIMERS_H_
