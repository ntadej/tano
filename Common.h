#ifndef COMMON_H_
#define COMMON_H_

#include <QString>
#include <QStringList>

class Common {
public:
	static QString locateResource(QString fileN);
	static QString locateLang(QString fileL);
	static QString settingsFile(QString type = "main");
	static QStringList settingsDefault(QString type = "main");
};

#endif /* COMMON_H_ */
