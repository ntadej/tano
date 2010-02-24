/****************************************************************************
* PlaylistWidget.cpp: Playlist display with search and categories support
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
#include <QtGui/QMessageBox>

#include "../xml/M3UGenerator.h"

#include "PlaylistWidget.h"

PlaylistWidget::PlaylistWidget(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);
	ui.treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);

	_handler = new M3UHandler(ui.treeWidget);

	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SIGNAL(itemClicked(QTreeWidgetItem*, int)));
	connect(ui.categoryBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(processPlaylist()));
	connect(ui.searchEdit, SIGNAL(textChanged(QString)), this, SLOT(processPlaylist()));
}

PlaylistWidget::~PlaylistWidget()
{
	delete _handler;
}

void PlaylistWidget::clear()
{
	_handler->clear();
}

void PlaylistWidget::open(const QString &file)
{
	_handler->clear();

	if (file.isEmpty())
		return;

	_fileName = file;

	QFile f(_fileName);
	if (!f.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Tano"),
							tr("Cannot read file %1:\n%2.")
							.arg(_fileName)
							.arg(f.errorString()));
		return;
	}

	_handler->processFile(_fileName);

	ui.categoryBox->clear();
	ui.categoryBox->insertItem(0,tr("All channels"));
	ui.categoryBox->insertItems(1,_handler->categories());

	ui.treeWidget->sortByColumn(0, Qt::AscendingOrder);
}

void PlaylistWidget::save(const QString &name, const QString &file)
{
	QFile f(file);
	if (!f.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Tano"),
							tr("Cannot write file %1:\n%2.")
							.arg(file)
							.arg(f.errorString()));
		return;
	}

	M3UGenerator *generator = new M3UGenerator(ui.treeWidget, name, _handler->channelMap());
	generator->write(&f);
	delete generator;
}

void PlaylistWidget::processPlaylist()
{
	for(int i=0; i<ui.treeWidget->topLevelItemCount(); i++)
		ui.treeWidget->topLevelItem(i)->setHidden(false);

	if(ui.categoryBox->currentText() != tr("All channels"))
		for(int i=0; i<ui.treeWidget->topLevelItemCount(); i++)
			if(!ui.treeWidget->topLevelItem(i)->text(2).contains(ui.categoryBox->currentText(), Qt::CaseInsensitive))
				ui.treeWidget->topLevelItem(i)->setHidden(true);

	if(ui.searchEdit->text() != "")
			for(int i=0; i<ui.treeWidget->topLevelItemCount(); i++)
				if(!ui.treeWidget->topLevelItem(i)->text(1).contains(ui.searchEdit->text(), Qt::CaseInsensitive))
					ui.treeWidget->topLevelItem(i)->setHidden(true);

}

QTreeWidgetItem *PlaylistWidget::createItem()
{
	QTreeWidgetItem *newI = _handler->createChannel();
	ui.treeWidget->sortByColumn(0, Qt::AscendingOrder);
	return newI;
}

void PlaylistWidget::deleteItem()
{
	_handler->deleteChannel(ui.treeWidget->currentItem());
	ui.treeWidget->sortByColumn(0, Qt::AscendingOrder);
}

Channel *PlaylistWidget::channelRead(QTreeWidgetItem* clickedChannel)
{
	return _handler->channelRead(clickedChannel);
}

Channel *PlaylistWidget::channelRead(const int &clickedChannel)
{
	return _handler->channelRead(clickedChannel);
}

void PlaylistWidget::import(const QString &file)
{
	_handler->clear();
	_handler->importOldFormat(file);
}

int PlaylistWidget::processNum(QTreeWidgetItem *channel, const int &num)
{
	return _handler->processNewNum(channel, num);
}

void PlaylistWidget::disableCategories()
{
	ui.labelCategory->hide();
	ui.categoryBox->hide();
}
