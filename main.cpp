#include "Tano.h"
#include "Common.h"
#include "settings/SettingsMain.h"

#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>
#include <QtDebug>
#include <QPlastiqueStyle>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Tano Player");

#ifdef Q_WS_WIN
	QApplication::setStyle(new QPlastiqueStyle);
#endif

    SettingsMain *settings = new SettingsMain(Common::settingsFile(), Common::settingsDefault());

    QString locale = settings->locale();
	QTranslator translator;

	QString langPath = Common::locateLang("tano_" + locale + ".qm");

	translator.load(QString("tano_" + locale), langPath);
	app.installTranslator(&translator);

    Tano mainWindow(0, settings->defaultPlaylist(), settings->session());
    mainWindow.show();
    return app.exec();
}
