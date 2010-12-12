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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "core/PluginsLoader.h"

#include "PluginsDialog.h"
#include "ui_PluginsDialog.h"

PluginsDialog::PluginsDialog(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::PluginsDialog)
{
	ui->setupUi(this);

	_interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
	_interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
	_featureIcon = QIcon(":/icons/24x24/plugin.png");

	_recorder = new QTreeWidgetItem(ui->pluginsWidget);
	_recorder->setText(0, tr("Recorder Plugins"));
	ui->pluginsWidget->setItemExpanded(_recorder, true);

	QFont font = _recorder->font(0);
	font.setBold(true);
	_recorder->setFont(0, font);

	PluginsLoader *loader = new PluginsLoader();

	for(int i=0; i < loader->recorderPlugin().size(); i++)
		populateTreeWidget(loader->recorderFile()[i], loader->recorderName()[i], "Recorder");

	delete loader;
}

PluginsDialog::~PluginsDialog()
{
	delete ui;
}

void PluginsDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void PluginsDialog::populateTreeWidget(const QString &file,
									   const QString &name,
									   const QString &type)
{
	QTreeWidgetItem *pluginItem;
	if(type == "Recorder")
		pluginItem = new QTreeWidgetItem(_recorder);
	pluginItem->setText(0, file);
	pluginItem->setIcon(0, _interfaceIcon);

	QTreeWidgetItem *featureItem = new QTreeWidgetItem(pluginItem);
	featureItem->setText(0, name);
	featureItem->setIcon(0, _featureIcon);
}
