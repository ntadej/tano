#ifndef TANO_PLUGINSMANAGER_H_
#define TANO_PLUGINSMANAGER_H_

#include <QtCore/QDir>
#include <QtGui/QDialog>
#include <QtGui/QIcon>
#include <ui_PluginsManager.h>

class PluginsManager : public QDialog
{
Q_OBJECT
public:
	PluginsManager(QWidget *parent = 0);
	~PluginsManager();

private:
	Ui::PluginsManager ui;

	void populateTreeWidget(QObject *plugin, const QString &text);

	QDir pluginsDir;
	QStringList pluginFileNames;

	QIcon interfaceIcon;
	QIcon featureIcon;
};

#endif // TANO_PLUGINSMANAGER_H_
