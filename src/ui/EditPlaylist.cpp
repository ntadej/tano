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
#include "plugins/PluginsLoader.h"

EditPlaylist::EditPlaylist(const QString &playlist, QWidget *parent)
	: QMainWindow(parent), _closeEnabled(false), _playlist(playlist)
{
	ui.setupUi(this);

	ui.editWidget->setEnabled(false);

	connect(ui.actionDelete, SIGNAL(triggered()), this, SLOT(deleteItem()));
	connect(ui.actionAdd, SIGNAL(triggered()), this, SLOT(addItem()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(exit()));
	connect(ui.actionImport, SIGNAL(triggered()), this, SLOT(import()));

	connect(ui.buttonApplyNum, SIGNAL(clicked()), this, SLOT(editChannelNumber()));
	connect(ui.editChannelName, SIGNAL(textChanged(QString)), this, SLOT(editChannelName(QString)));
	connect(ui.editUrl, SIGNAL(textChanged(QString)), this, SLOT(editChannelUrl(QString)));
	connect(ui.editCategories, SIGNAL(textChanged(QString)), this, SLOT(editChannelCategories(QString)));
	connect(ui.editLanguage, SIGNAL(textChanged(QString)), this, SLOT(editChannelLanguage(QString)));
	connect(ui.editEpg, SIGNAL(textChanged(QString)), this, SLOT(editChannelEpg(QString)));

	connect(ui.playlist, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(editItem(QTreeWidgetItem*)));

	_channelIcon = QIcon(":/icons/images/video.png");

	ui.playlist->disableCategories();
	ui.playlist->open(_playlist);
	ui.editName->setText(ui.playlist->name());

	PluginsLoader *loader = new PluginsLoader();
	for(int i=0; i < loader->epgPlugin().size(); i++)
		ui.epgCombo->addItem(loader->epgName()[i]);
	delete loader;
	for(int i=0; i < ui.epgCombo->count(); i++)
		if(ui.epgCombo->itemText(i) == ui.playlist->epgPlugin()) {
			ui.epgCombo->setCurrentIndex(i);
			break;
		}
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
	ui.editNumber->setText("");
	ui.editChannelName->setText("");
	ui.editUrl->setText("");
	ui.editCategories->setText("");
	ui.editLanguage->setText("");
	ui.editEpg->setText("");

	ui.editWidget->setEnabled(false);

	ui.playlist->deleteItem();
}

void EditPlaylist::addItem()
{
	editItem(ui.playlist->createItem());
}

void EditPlaylist::save()
{
	QString fileName =
		QFileDialog::getSaveFileName(this, tr("Save Channel list"),
									QDir::homePath(),
									tr("Tano TV Channel list Files (*.m3u)"));
	if (fileName.isEmpty())
		return;

	ui.playlist->save(ui.editName->text(), ui.epgCombo->currentText(), fileName);
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

void EditPlaylist::editItem(QTreeWidgetItem *item)
{
	if(!ui.editWidget->isEnabled())
		ui.editWidget->setEnabled(true);

	ui.playlist->treeWidget()->setCurrentItem(item);

	ui.editNumber->setText(ui.playlist->channelRead(item)->numberString());
	ui.editChannelName->setText(ui.playlist->channelRead(item)->name());
	ui.editUrl->setText(ui.playlist->channelRead(item)->url());
	ui.editCategories->setText(ui.playlist->channelRead(item)->categories().join(","));
	ui.editLanguage->setText(ui.playlist->channelRead(item)->language());
	ui.editEpg->setText(ui.playlist->channelRead(item)->epg());
}

void EditPlaylist::editChannelNumber()
{
	QString text = ui.editNumber->text();
	if(text.toInt() != ui.playlist->channelRead(ui.playlist->treeWidget()->currentItem())->number())
		ui.editNumber->setText(QString().number(ui.playlist->processNum(ui.playlist->treeWidget()->currentItem(), text.toInt())));
}

void EditPlaylist::editChannelName(const QString &text)
{
	ui.playlist->channelRead(ui.playlist->treeWidget()->currentItem())->setName(text);
	ui.playlist->treeWidget()->currentItem()->setText(1, text);
}

void EditPlaylist::editChannelUrl(const QString &text)
{
	ui.playlist->channelRead(ui.playlist->treeWidget()->currentItem())->setUrl(text);
}

void EditPlaylist::editChannelCategories(const QString &text)
{
	ui.playlist->channelRead(ui.playlist->treeWidget()->currentItem())->setCategories(text.split(","));
}

void EditPlaylist::editChannelLanguage(const QString &text)
{
	ui.playlist->channelRead(ui.playlist->treeWidget()->currentItem())->setLanguage(text);
}

void EditPlaylist::editChannelEpg(const QString &text)
{
	ui.playlist->channelRead(ui.playlist->treeWidget()->currentItem())->setEpg(text);
}
