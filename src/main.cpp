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
#include <QtCore/QLocale>
#include <QtCore/QTranslator>

#include "MainWindow.h"
#include "Ver.h"
#include "core/Common.h"
#include "core/Settings.h"
#include "ui/FirstRun.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Tano");

	QString locale;
	Settings *settings = new Settings();
	if(settings->language().isEmpty())
		locale = QLocale::system().name();
	else
		locale = settings->language();

    QTranslator translator;
	QString langPath = Common::locateLang("tano_" + locale + ".qm");
	translator.load(QString("tano_" + locale), langPath);
	app.installTranslator(&translator);

	if(!settings->configured() || settings->configurationVersion() != Version::Tano()) {
		FirstRun *wizard = new FirstRun();
		wizard->exec();
		delete wizard;
	}

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
