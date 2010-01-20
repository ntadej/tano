#ifndef TANO_M3UHANDLER_H_
#define TANO_M3UHANDLER_H_

#include <QIcon>
#include <QMap>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "../channels/Channel.h"

class M3UHandler {
public:
	M3UHandler(QTreeWidget *treeWidget, bool editable = false);
	virtual ~M3UHandler();

	void processFile(QString m3uFile);
	void clear();

    Channel *channelRead(QTreeWidgetItem *clickedItem);
    Channel *channelReadNum(int clickedItem);
    QString getName() const {return name;};
    QStringList getCategories() const {return categoryList;};
    QList<int> nums() const {return channelNums;};

private:
	void processList();

    QTreeWidgetItem *createChildItem(const QString &tagName);
    Channel *createChannel(QString name, int num, bool cat);

	bool valid;
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
