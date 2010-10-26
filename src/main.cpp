/****************************************************************************
* main.cpp: Tano application main
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

#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>

#include "Config.h"

#if UI_BASIC
	#include "MainWindow.h"
#elif UI_QML
	#include "qml/MainQml.h"
#endif

#include "core/Settings.h"
#include "core/Version.h"
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

#if UI_BASIC
	MainWindow mainWindow;
    mainWindow.show();
#elif UI_QML
	MainQml mainWindow;
	mainWindow.show();
#endif

    return app.exec();
}
