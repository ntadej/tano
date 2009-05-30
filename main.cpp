#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>
#include <QSettings>
#include <QLocale>
#include <QtDebug>

#include "MainWindow.h"
#include "Common.h"
#include "ui/FirstRun.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Tano Player");

    //Settings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Tano", "Settings");
    QString locale = settings.value("locale", QString(QLocale::system().name()).replace(2, 3, "")).toString();

    QTranslator translator;
	QString langPath = Common::locateLang("tano_" + locale + ".qm");
	translator.load(QString("tano_" + locale), langPath);
	app.installTranslator(&translator);

	if(!settings.value("registered",false).toBool()) {
		FirstRun wizard;
		wizard.exec();
	}

    MainWindow mainWindow;

    mainWindow.show();

    return app.exec();
}
