#ifndef COMMON_H_
#define COMMON_H_

#include <QString>
#include <QStringList>

class Common {
public:
	static QString locateResource(QString fileN);
	static QString locateLang(QString fileL);

	static QString version();

	static void about(QWidget *parent = 0 );
	static void help(QWidget *parent = 0 );
};

#endif /* COMMON_H_ */
