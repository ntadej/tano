/****************************************************************************
* PluginsManager.cpp: A dialog that lists all available plugins
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

#include "PluginsLoader.h"
#include "PluginsManager.h"

PluginsManager::PluginsManager(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	_interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
	_interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
	_featureIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

	PluginsLoader *loader = new PluginsLoader();

	for(int i=0; i < loader->recorderPlugin().size(); i++)
		populateTreeWidget(loader->recorderFile()[i], loader->recorderName()[i]);

	delete loader;
}

PluginsManager::~PluginsManager()
{

}

void PluginsManager::populateTreeWidget(const QString &file, const QString &name)
{
	QTreeWidgetItem *pluginItem = new QTreeWidgetItem(ui.pluginsWidget);
	pluginItem->setText(0, file);
	ui.pluginsWidget->setItemExpanded(pluginItem, true);

	QFont boldFont = pluginItem->font(0);
	boldFont.setBold(true);
	pluginItem->setFont(0, boldFont);

	QTreeWidgetItem *interfaceItem = new QTreeWidgetItem(pluginItem);
	interfaceItem->setText(0, tr("Recorder Plugin"));
	interfaceItem->setIcon(0, _interfaceIcon);
	QTreeWidgetItem *featureItem = new QTreeWidgetItem(interfaceItem);
	featureItem->setText(0, name);
	featureItem->setIcon(0, _featureIcon);
}
