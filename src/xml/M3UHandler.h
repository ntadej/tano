#ifndef TANO_M3UHANDLER_H_
#define TANO_M3UHANDLER_H_

#include <QtCore/QMap>
#include <QtGui/QIcon>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

#include "../channels/Channel.h"

class M3UHandler
{
public:
	M3UHandler(QTreeWidget *treeWidget, const bool &editable = false);
	virtual ~M3UHandler();

	void processFile(const QString &m3uFile);
	void clear();

	Channel *channelRead(QTreeWidgetItem *clickedItem);
	Channel *channelRead(const int &clickedItem);
	QString getName() const {return name;};
	QStringList getCategories() const {return categoryList;};
	QList<int> nums() const {return channelNums;};

private:
	void processList();

	bool edit;

	QTreeWidget *treeWidget;
	QTreeWidgetItem *item;
	Channel *channel;

	QIcon channelIcon;

	QString name;
	QStringList m3uLineList;
	QStringList categoryList;

	QList<int> channelNums;

	QMap<QTreeWidgetItem*, Channel*> map;
	QMap<int, Channel*> nmap;
};

#endif // TANO_M3UHANDLER_H_
