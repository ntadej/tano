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

	QTreeWidgetItem *_recorder;
};

#endif // TANO_PLUGINSDIALOG_H_
