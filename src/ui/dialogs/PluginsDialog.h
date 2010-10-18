/****************************************************************************
* PluginsDialog.h: A dialog that lists all available plugins
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

#ifndef TANO_PLUGINSDIALOG_H_
#define TANO_PLUGINSDIALOG_H_

#include <QtCore/QDir>
#include <QtGui/QDialog>
#include <QtGui/QIcon>
#include <QtGui/QTreeWidgetItem>

namespace Ui
{
	class PluginsDialog;
}

class PluginsDialog : public QDialog
{
Q_OBJECT
public:
	PluginsDialog(QWidget *parent = 0);
	~PluginsDialog();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::PluginsDialog *ui;

	void populateTreeWidget(const QString &file,
							const QString &name,
							const QString &type);

	QDir _pluginsDir;
	QStringList _pluginFileNames;

	QIcon _interfaceIcon;
	QIcon _featureIcon;

	QTreeWidgetItem *_epg;
	QTreeWidgetItem *_recorder;
};

#endif // TANO_PLUGINSDIALOG_H_
