/****************************************************************************
* EditPlaylist.cpp: Playlist editor
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

#include <QtGui/QCloseEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include "EditPlaylist.h"

EditPlaylist::EditPlaylist(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	_closeEnabled = false;

	connect(ui.actionDelete, SIGNAL(triggered()), this, SLOT(deleteItem()));
	connect(ui.actionAddChannel, SIGNAL(triggered()), this, SLOT(addItem()));
	connect(ui.actionReload, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(exit()));
	connect(ui.actionImport, SIGNAL(triggered()), this, SLOT(import()));

	_channelIcon = QIcon(":/icons/images/video.png");
}

EditPlaylist::~EditPlaylist()
{

}

void EditPlaylist::closeEvent(QCloseEvent *event)
{
	if(!_closeEnabled) {
		event->ignore();
	} else {
		_closeEnabled = false;
	}
}

void EditPlaylist::deleteItem()
{
	ui.playlist->deleteItem();
}

void EditPlaylist::addItem()
{
	ui.playlist->createItem();
}

void EditPlaylist::save()
{
	/*QString fileName =
		QFileDialog::getSaveFileName(this, tr("Save Channel list"),
									QDir::homePath(),
									tr("Tano TV Channel list Files (*.m3u)"));
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

	M3UGenerator *generator = new M3UGenerator(ui.playlist, ui.editName->text());
	generator->write(&file);
	delete generator;*/
}

void EditPlaylist::open()
{
	ui.playlist->open(_playlist);
	ui.editName->setText(ui.playlist->name());
	show();
}

void EditPlaylist::import()
{
	QString fileName =
			QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
										QDir::homePath(),
										tr("Tano TV Old Channel list Files(*.tano *.xml)"));
	if (fileName.isEmpty())
		return;

	ui.playlist->import(fileName);
}

void EditPlaylist::setPlaylist(const QString &file)
{
	_playlist = file;
}

void EditPlaylist::exit()
{
	if(_closeEnabled) {
		close();
		return;
	}

	int ret;
	ret = QMessageBox::warning(this, ui.editName->text(),
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
