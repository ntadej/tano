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
#include <QtCore/QSettings>
#include <QtCore/QTranslator>

#include "Common.h"
#include "Ver.h"
#include "MainWindow.h"
#include "ui/FirstRun.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Tano");

    //Settings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Tano", "Settings");
    QString locale = settings.value("locale", QString(QLocale::system().name()).replace(2, 3, "")).toString();

    QTranslator translator;
	QString langPath = Common::locateLang("tano_" + locale + ".qm");
	translator.load(QString("tano_" + locale), langPath);
	app.installTranslator(&translator);

	if(!settings.value("registered",false).toBool() || settings.value("version",Version::Tano()).toString() != Version::Tano()) {
		FirstRun *wizard = new FirstRun();
		wizard->exec();
		delete wizard;
	}

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
