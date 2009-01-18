#ifndef COMMON_H_
#define COMMON_H_

#include <QString>

class Common {
public:
	static QString locateResource(QString fileN);
	static QString locateLang(QString fileL);
	static QString settingsFile();
	static QString vlcStatus();
};

#endif /* COMMON_H_ */
