/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_TIMERSEDIT_H_
#define TANO_TIMERSEDIT_H_

#include <QtCore/QMap>
#include <QtCore/QSettings>
#include <QtGui/QMainWindow>
#include <QtGui/QTreeWidgetItem>

#include "container/Channel.h"
#include "container/Timer.h"
#include "xml/TimersHandler.h"

namespace Ui
{
	class TimersEdit;
}

class TimersEdit : public QMainWindow
{
Q_OBJECT
public:
	TimersEdit(const QString &playlist,
			   QWidget *parent = 0);
	~TimersEdit();

protected:
	void changeEvent(QEvent *e);
	void closeEvent(QCloseEvent *event);

private slots:
	void addItem();
	void deleteItem();
	void newItem();

	void edit(QTreeWidgetItem *item);
	void playlist(QTreeWidgetItem *item);

	void validate();

	void editName(const QString &name);
	void editType(const int &type);
	void editDate(const QDate &date);
	void editStartTime(const QTime &time);
	void editEndTime(const QTime &time);

	void read();
	void write();
	void exit();

private:
	void createConnections();
	void createSettings();

	Ui::TimersEdit *ui;

	Channel *_channel;

	bool _closeEnabled;

	TimersHandler *_handler;

	QString _path;
	QString _playlist;
};

#endif // TANO_TIMERSEDIT_H_
