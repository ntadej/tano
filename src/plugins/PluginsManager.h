/****************************************************************************
* PluginsManager.h: A dialog that lists all available plugins
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

#ifndef TANO_PLUGINSMANAGER_H_
#define TANO_PLUGINSMANAGER_H_

#include <QtCore/QDir>
#include <QtGui/QDialog>
#include <QtGui/QIcon>

#include <ui_PluginsManager.h>

class PluginsManager : public QDialog
{
Q_OBJECT
public:
	PluginsManager(QWidget *parent = 0);
	~PluginsManager();

private:
	Ui::PluginsManager ui;

	void populateTreeWidget(const QString &file, const QString &name);

	QDir _pluginsDir;
	QStringList _pluginFileNames;

	QIcon _interfaceIcon;
	QIcon _featureIcon;
};

#endif // TANO_PLUGINSMANAGER_H_
