/****************************************************************************
* EditTimers.cpp: Timers editor and manager
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

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtGui/QCloseEvent>
#include <QtGui/QMessageBox>
#include <QtXml/QXmlSimpleReader>
#include <QtXml/QXmlInputSource>

#include "EditTimers.h"
#include "core/Settings.h"
#include "xml/TimersGenerator.h"


EditTimers::EditTimers(Time *t, const QString &playlist, QWidget *parent)
	: QMainWindow(parent), _time(t), _channel(0), _activeTimers(false), _closeEnabled(false), _path("")
{
	ui.setupUi(this);
	ui.timersWidget->header()->setResizeMode(QHeaderView::ResizeToContents);

	ui.dockWidgetContents->setDisabled(true);
	ui.playlistWidget->open(playlist);

	createSettings();
	createConnections();

	_handler = new TimersHandler(ui.timersWidget);

	read();
}

EditTimers::~EditTimers()
{

}

void EditTimers::closeEvent(QCloseEvent *event)
{
	if(!_closeEnabled) {
		event->ignore();
	} else {
		_closeEnabled = false;
	}
}

void EditTimers::createSettings()
{
	Settings *settings = new Settings(this);
	ui.toolBar->setToolButtonStyle(Qt::ToolButtonStyle(settings->toolbarLook()));
	_path = settings->path();
	delete settings;
}

void EditTimers::createConnections()
{
	connect(ui.timersWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(edit(QTreeWidgetItem*)));
	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(newItem()));
	connect(ui.actionDelete, SIGNAL(triggered()), this, SLOT(deleteItem()));
	connect(ui.buttonCreate, SIGNAL(clicked()), this, SLOT(addItem()));
	connect(ui.playlistWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playlist(QTreeWidgetItem*)));

	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(write()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(exit()));

	connect(ui.editName, SIGNAL(textChanged(QString)), this, SLOT(editName(QString)));
	connect(ui.editDate, SIGNAL(dateChanged(QDate)), this, SLOT(editDate(QDate)));
	connect(ui.editStartTime, SIGNAL(timeChanged(QTime)), this, SLOT(editStartTime(QTime)));
	connect(ui.editEndTime, SIGNAL(timeChanged(QTime)), this, SLOT(editEndTime(QTime)));
	connect(ui.checkBoxDisabled, SIGNAL(clicked()), this, SLOT(validate()));

	connect(_time, SIGNAL(timerStatus(Timer*, bool)), this, SLOT(changeStatus(Timer*, bool)));
}

void EditTimers::exit()
{
	if(_closeEnabled) {
		close();
		return;
	}

	int ret;
	ret = QMessageBox::warning(this, tr("Timers"),
								   tr("Do you want close the editor?\nYou will lose any unsaved settings."),
								   QMessageBox::Close | QMessageBox::Cancel,
								   QMessageBox::Close);

	switch (ret) {
		case QMessageBox::Close:
			_closeEnabled = true;
			ui.actionClose->trigger();
			break;
		case QMessageBox::Cancel:
			break;
		default:
			break;
	}
}

void EditTimers::newItem()
{
	ui.toolBar->setDisabled(true);
	ui.dockWidgetContents->setDisabled(true);
	ui.mainWidget->setCurrentIndex(1);
}

void EditTimers::deleteItem()
{
	_time->removeTimer(_handler->timerRead(ui.timersWidget->currentItem()));
	ui.dockWidgetContents->setDisabled(true);
	_handler->deleteItem(ui.timersWidget->currentItem());
}

void EditTimers::addItem()
{
	if(_channel == 0 || ui.editNameNew->text() == "") {
		QMessageBox::warning(this, tr("Tano"),
							tr("Please enter a name and select a channel from the list."));
		return;
	} else {
		for(int i=0; i<ui.timersWidget->topLevelItemCount(); i++) {
			if(ui.timersWidget->topLevelItem(i)->text(0) == ui.editNameNew->text()) {
				QMessageBox::warning(this, tr("Tano"),
									tr("Timer with this name already exists. Please select another name."));
				return;
			}
		}
	}

	edit(_handler->newTimer(ui.editNameNew->text(),_channel->name(),ui.playlistWidget->fileName(),_channel->number(),_channel->url()));

	ui.dockWidgetContents->setDisabled(false);
	ui.toolBar->setDisabled(false);
	ui.mainWidget->setCurrentIndex(0);

	delete _channel;
}

void EditTimers::playlist(QTreeWidgetItem *item)
{
	if(_channel != 0)
		delete _channel;

	_channel = ui.playlistWidget->channelRead(item);
}

void EditTimers::edit(QTreeWidgetItem *item)
{
	ui.dockWidgetContents->setDisabled(false);

	if(_handler->timerRead(item)->isActive()) {
		ui.dockWidgetContents->setDisabled(true);
		return;
	}

	ui.timersWidget->setCurrentItem(item);

	ui.checkBoxDisabled->setChecked(_handler->timerRead(item)->isDisabled());
	ui.editName->setText(_handler->timerRead(item)->name());
	ui.editChannel->setText(_handler->timerRead(item)->channel());
	ui.editNum->display(_handler->timerRead(item)->num());
	ui.editPlaylist->setText(_handler->timerRead(item)->playlist());
	ui.editUrl->setText(_handler->timerRead(item)->url());
	ui.editDate->setDate(_handler->timerRead(item)->date());
	ui.editStartTime->setTime(_handler->timerRead(item)->startTime());
	ui.editEndTime->setTime(_handler->timerRead(item)->endTime());
}

void EditTimers::editName(const QString &name)
{
	for(int i=0; i<ui.timersWidget->topLevelItemCount(); i++)
		if(ui.timersWidget->topLevelItem(i)->text(0) == name && ui.timersWidget->topLevelItem(i) != ui.timersWidget->currentItem()) {
			QMessageBox::warning(this, tr("Tano"),
								tr("Timer with this name already exists. Please select another name."));
			ui.editName->setText(ui.timersWidget->currentItem()->text(0));
			_handler->timerRead(ui.timersWidget->currentItem())->setName(ui.timersWidget->currentItem()->text(0));
			return;
		}

	ui.timersWidget->currentItem()->setText(0,name);
	_handler->timerRead(ui.timersWidget->currentItem())->setName(name);
}

void EditTimers::editDate(const QDate &date)
{
	_handler->timerRead(ui.timersWidget->currentItem())->setDate(date);
}

void EditTimers::editStartTime(const QTime &time)
{
	_handler->timerRead(ui.timersWidget->currentItem())->setStartTime(time);
	validate();
}

void EditTimers::editEndTime(const QTime &time)
{
	_handler->timerRead(ui.timersWidget->currentItem())->setEndTime(time);
	validate();
}

void EditTimers::read(const QString &file)
{
	_activeTimers = false;

	QString fileName;

	if(file.isNull())
		fileName = _path + "timers.tano.xml";
	else
		fileName = file;

	if (fileName.isEmpty())
		return;

	QXmlSimpleReader reader;
	reader.setContentHandler(_handler);
	reader.setErrorHandler(_handler);

	if(!QFileInfo(fileName).exists())
		return;

	QFile fileR(fileName);
	if (!fileR.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Tano"),
							tr("Cannot read file %1:\n%2.")
							.arg(fileName)
							.arg(fileR.errorString()));
		return;
	}

	QXmlInputSource xmlInputSource(&fileR);
	if (!reader.parse(xmlInputSource))
		return;

	for(int i=0; i<ui.timersWidget->topLevelItemCount(); i++) {
		if(!_handler->timerRead(ui.timersWidget->topLevelItem(i))->isDisabled())
			_time->addTimer(_handler->timerRead(ui.timersWidget->topLevelItem(i)));
	}

	_activeTimers = true;
}

void EditTimers::write()
{
	QString fileName;

	/*fileName =
		QFileDialog::getSaveFileName(this, tr("Save Timers"),
									QDir::homePath(),
									tr("Tano Timers (*.tano.xml)"));
	*/

	fileName = _path + "timers.tano.xml";

	if (fileName.isEmpty())
		return;

	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Tano"),
							tr("Cannot write file %1:\n%2.")
							.arg(fileName)
							.arg(file.errorString()));
		return;
	}

	TanoGenerator *generator = new TanoGenerator(ui.timersWidget, _handler->timersMap());
	generator->write(&file);
	delete generator;

	for(int i=0; i<ui.timersWidget->topLevelItemCount(); i++) {
		if(!_handler->timerRead(ui.timersWidget->topLevelItem(i))->isDisabled())
			_time->addTimer(_handler->timerRead(ui.timersWidget->topLevelItem(i)));
	}
}

void EditTimers::changeStatus(Timer *t, const bool &status)
{
	if(status) {
		_handler->itemRead(t)->setText(1,tr("Recording"));
		if(ui.timersWidget->currentItem() == _handler->itemRead(t))
			ui.dockWidgetContents->setDisabled(true);
	}
}

void EditTimers::validate()
{
	if(!_activeTimers)
		return;

	if(ui.editDate->date() < QDate::currentDate() ||
		ui.editEndTime->time() < QTime::currentTime() ||
		ui.checkBoxDisabled->isChecked())
	{
		_handler->timerRead(ui.timersWidget->currentItem())->setDisabled(true);
		ui.timersWidget->currentItem()->setText(1,tr("Disabled or expired"));
		_time->removeTimer(_handler->timerRead(ui.timersWidget->currentItem()));
	} else {
		_handler->timerRead(ui.timersWidget->currentItem())->setDisabled(false);
		ui.timersWidget->currentItem()->setText(1,tr("Active"));
	}
}
