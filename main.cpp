#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>
#include <QSettings>
#include <QLocale>
#include <QtDebug>

#include "Tano.h"
#include "Common.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Tano Player");

    //Settings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Tano", "Settings");
    QString locale = settings.value("locale", QString(QLocale::system().name()).replace(2, 3, "")).toString();
	QString playlist = settings.value("playlist","siol.xml").toString();
	bool session = settings.value("session", true).toBool();

    QTranslator translator;
	QString langPath = Common::locateLang("tano_" + locale + ".qm");
	translator.load(QString("tano_" + locale), langPath);
	app.installTranslator(&translator);

    Tano mainWindow(0, playlist, session);
    mainWindow.show();
    return app.exec();
}
