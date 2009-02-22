#ifndef COMMON_H_
#define COMMON_H_

#include <QString>
#include <QStringList>

class Common {
public:
	static QString locateResource(QString fileN);
	static QString locateLang(QString fileL);
	static QString settingsMainFile();
	static QStringList settingsMainDefault();
	static QString settingsShortcutsFile();
};

#endif /* COMMON_H_ */
