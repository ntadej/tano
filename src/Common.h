#ifndef TANO_COMMON_H_
#define TANO_COMMON_H_

#include <QString>
#include <QStringList>
#include <QSettings>

class Common {
public:
	static QString locateResource(QString fileN);
	static QString locateLang(QString fileL);

	static QString version();

	static bool fripExists();
	static QString frip();

	static void about(QWidget *parent = 0 );

	static QSettings* settings();
	static QString settingsPath();

	static QStringList libvlcArgs();
};

#endif // TANO_COMMON_H_
