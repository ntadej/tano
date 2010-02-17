#include <QtCore/QPluginLoader>
#include <QtCore/QDebug>

#include "Plugins.h"
#include "PluginsManager.h"

PluginsManager::PluginsManager(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
							QIcon::Normal, QIcon::On);
	interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
							QIcon::Normal, QIcon::Off);
	featureIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

	pluginsDir = QDir(qApp->applicationDirPath());
	pluginsDir.cdUp();
	pluginsDir.cd("plugins");

	QDir tmp;
	foreach (QString folderName, pluginsDir.entryList(QDir::Dirs)) {
		tmp = pluginsDir;
		tmp.cd(folderName);
		foreach (QString fileName, tmp.entryList(QDir::Files)) {
			QPluginLoader loader;
			loader.setFileName(tmp.absoluteFilePath(fileName));
			QObject *plugin = loader.instance();
			if (plugin) {
				pluginFileNames += fileName;
				populateTreeWidget(plugin, fileName);
			}
		}
	}

}

PluginsManager::~PluginsManager()
{

}

void PluginsManager::populateTreeWidget(QObject *plugin, const QString &text)
{
	QTreeWidgetItem *pluginItem = new QTreeWidgetItem(ui.pluginsWidget);
	pluginItem->setText(0, text);
	ui.pluginsWidget->setItemExpanded(pluginItem, true);

	QFont boldFont = pluginItem->font(0);
	boldFont.setBold(true);
	pluginItem->setFont(0, boldFont);

	if (plugin) {
		RecorderPluginCreator *rp =
				qobject_cast<RecorderPluginCreator *>(plugin);
		if (rp) {
			QTreeWidgetItem *interfaceItem = new QTreeWidgetItem(pluginItem);
			interfaceItem->setText(0, "Recorder");
			interfaceItem->setIcon(0, interfaceIcon);
			QTreeWidgetItem *featureItem = new QTreeWidgetItem(interfaceItem);
			featureItem->setText(0, rp->createRecorderPluginInstance()->name());
			featureItem->setIcon(0, featureIcon);
		}
	}
}
