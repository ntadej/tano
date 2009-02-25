#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QMap>
#include <QCoreApplication>

#include "Common.h"

QString Common::locateResource(QString fileN) {
	QString path;

	if (QFileInfo(fileN).exists())
		path = QFileInfo(fileN).absoluteFilePath();

	// Try application exe working path
	else if (QFileInfo(QDir::currentPath() + "/" + fileN).exists())
		path = QFileInfo(QDir::currentPath() + "/" + fileN).absoluteFilePath();

	// Try application exe directory
	else if (QFileInfo(QCoreApplication::applicationDirPath() + "/" + fileN).exists())
		path = QFileInfo(QCoreApplication::applicationDirPath() + "/" + fileN).absoluteFilePath();

#ifdef DEFAULT_DATA_DIR
	else if (QFileInfo(QString(DEFAULT_DATA_DIR) + "/" + fileN).exists())
		path = QFileInfo(QString(DEFAULT_DATA_DIR) + "/" + fileN).absoluteFilePath();
#endif

	return path;
}

QString Common::locateLang(QString fileL) {
	QString path;

	if (QFileInfo(fileL).exists())
		path = QFileInfo(fileL).absoluteFilePath();

	// Try application exe working path
	else if (QFileInfo(QDir::currentPath() + "/lang/" + fileL).exists())
		path = QFileInfo(QDir::currentPath() + "/lang/" + fileL).absoluteFilePath();

	// Try application exe directory
	else if (QFileInfo(QCoreApplication::applicationDirPath() + "/lang/" + fileL).exists())
		path = QFileInfo(QCoreApplication::applicationDirPath() + "/lang/" + fileL).absoluteFilePath();

#ifdef DEFAULT_DATA_DIR

	else if (QFileInfo(QString(DEFAULT_DATA_DIR) + "/lang/" + fileL).exists())
		path = QFileInfo(QString(DEFAULT_DATA_DIR) + "/lang/" + fileL).absoluteFilePath();

#endif

	return path.replace(QString("/" + fileL), QString(""));
}

QString Common::settingsFile(QString type)
{
	QMap<QString, int> map;
	map.insert("main", 1);
	map.insert("session", 2);
	map.insert("shortcuts", 3);

	QDir path(QDir::homePath());
	if (!QDir(QDir::homePath() + "/.tano/").exists())
		path.mkpath(QDir::homePath() + "/.tano/");

	switch(map[type]){
		case 1:
			return QDir::homePath() + "/.tano/settings";
			break;
		case 2:
			return QDir::homePath() + "/.tano/session";
			break;
		case 3:
			return QDir::homePath() + "/.tano/shortcuts";
			break;
		default:
			return QDir::homePath() + "/.tano/settings";
			break;
	}
}

QStringList Common::settingsDefault(QString type)
{
	QStringList defaultList;
	QMap<QString, int> map;
	map.insert("main", 1);
	map.insert("session", 2);
	map.insert("shortcuts", 3);

	switch(map[type]){
		case 1:
			defaultList << "Default" << "" << "";
			break;
		case 2:
			defaultList << "-";
			break;
		default:
			defaultList << "Default" << "" << "";
			break;
	}

	return defaultList;
}
