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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>

#include "core/Settings.h"
#include "core/Version.h"
#include "ui/MainWindow.h"
#include "ui/wizard/FirstRunWizard.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QCoreApplication::setApplicationName("Tano");

	Settings *settings = new Settings();
	if(!settings->configured() || settings->configurationVersion() != Version::version()) {
		FirstRunWizard *wizard = new FirstRunWizard();
		wizard->exec();
		delete wizard;
	}
	delete settings;

	MainWindow mainWindow;
	mainWindow.show();

	return app.exec();
}
