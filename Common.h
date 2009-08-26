#ifndef COMMON_H_
#define COMMON_H_

#include <QString>
#include <QStringList>
#include <QSettings>

extern bool isLocked;

class Common {
public:
	static QString locateResource(QString fileN);
	static QString locateLang(QString fileL);

	static QString version();

	static bool fripExists();
	static QString frip();

	static void about(QWidget *parent = 0 );
	static void help(QWidget *parent = 0 );

	static QSettings* settings();
};

#endif /* COMMON_H_ */
