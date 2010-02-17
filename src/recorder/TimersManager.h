#ifndef TANO_TIMERSMANAGER_H_
#define TANO_TIMERSMANAGER_H_

#include <QtCore/QMap>
#include <QtCore/QSettings>
#include <QtGui/QMainWindow>
#include <QtGui/QTreeWidgetItem>

#include "Timer.h"
#include "../control/Time.h"
#include "../channels/Channel.h"
#include "../xml/TimersHandler.h"

#include <ui_TimersManager.h>

class TimersManager : public QMainWindow
{
Q_OBJECT
public:
	TimersManager(Time *t, QWidget *parent = 0);
	~TimersManager();

	void openPlaylist(const QString &file);

public slots:
	void setStatus(Timer* t, const QString &status);

private slots:
	void action(QAbstractButton*);

	void newItem();
	void addItem();
	void deleteItem();
	void edit(QTreeWidgetItem *item);
	void playlist(QTreeWidgetItem *item);
	void applyName(const QString &name);
	void validate();

	void read(const QString &file = 0);
	void write();

private:
	Ui::TimersManager ui;

	Time *time;
	Timer *currentTimer;
	QTreeWidgetItem *currentItem;
	Channel *channel;

	TimersHandler *handler;
};

#endif // TANO_TIMERSMANAGER_H_
