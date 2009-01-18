#include "Tano.h"
#include "Common.h"
#include "settings/Settings.h"

#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>
#include <QtDebug>
#include <QPlastiqueStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("Tano Player");
	
#ifdef Q_WS_WIN
	QApplication::setStyle(new QPlastiqueStyle);
#endif

    Settings *settings = new Settings(Common::settingsFile());

    QString locale = settings->locale();
	QTranslator translator;

	QString langPath = Common::locateLang("tano_" + locale + ".qm");

	translator.load(QString("tano_" + locale), langPath);
	a.installTranslator(&translator);
	
    Tano w(0, settings->defaultPlaylist());
    w.show();
    return a.exec();
}
