#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "M3UHandler.h"

M3UHandler::M3UHandler(QTreeWidget *treeWidget, bool editable)
	: treeWidget(treeWidget)
{
	valid = false;
	edit = editable;

	name = QObject::tr("Channel list");

	categoryIcon.addPixmap(QPixmap(":/icons/images/folder.png"),
	                     QIcon::Normal, QIcon::Off);
	categoryIcon.addPixmap(QPixmap(":/icons/images/folder_video.png"),
	                     QIcon::Normal, QIcon::On);
	channelIcon = QIcon(":/icons/images/video.png");
}

M3UHandler::~M3UHandler() {

}

void M3UHandler::processFile(QString m3uFile)
{
	QFile file(m3uFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	while (!in.atEnd()) {
		m3uLineList << in.readLine();
	}

	processList();
}

void M3UHandler::clear()
{
	name = QObject::tr("Channel list");
	valid = false;
	m3uLineList.clear();
}

void M3UHandler::processList()
{
	QString tmp;
	QStringList tmpList;

	if(!m3uLineList.at(0).contains("#EXTM3U"))
		return;

	for(int i=1; i<m3uLineList.size(); i++) {
		if(m3uLineList.at(i)=="")
			continue;

		if(m3uLineList.at(i).contains("#EXTNAME")) {
			name = m3uLineList.at(i);
			name.replace(QString("#EXTNAME:"),QString(""));
		} else if(m3uLineList.at(i).contains("#EXTINF")) {
			tmp = m3uLineList.at(i);
			tmp.replace(QString("#EXTINF:"),QString(""));
			tmpList = tmp.split(",");

			item = new QTreeWidgetItem(treeWidget);
			item->setData(0, Qt::UserRole, "channel");
			item->setFlags(item->flags() | Qt::ItemIsEditable);
			item->setIcon(0, channelIcon);
			item->setText(0, tmpList.at(1));
			channel = createChannel(tmpList.at(1), tmpList.at(0).toInt(), false);

			map.insert(item, channel);
			nmap.insert(tmpList.at(0).toInt(), channel);
		} else if(m3uLineList.at(i).contains("#EXTTV")) {
			tmp = m3uLineList.at(i);
			tmp.replace(QString("#EXTTV:"),QString(""));
			tmpList = tmp.split(";");

			if(tmpList.size()!=0) {
				channel->setLanguage(tmpList.at(1));
				channel->setEpg(tmpList.at(2));
			}

			//processCategory(channel, tmpList.at(0));
		} else {
			tmp = m3uLineList.at(i);
			channel->setUrl(tmp);
		}
	}
}

Channel *M3UHandler::createChannel(QString name, int num, bool cat)
{
    Channel *childChannel = new Channel(name,num,cat);
    return childChannel;
}

Channel *M3UHandler::channelRead(QTreeWidgetItem *clickedItem)
{
	Channel *newChannel = map[clickedItem];

	return newChannel;
}

Channel *M3UHandler::channelReadNum(int clickedItem)
{
	Channel *newChannel = nmap[clickedItem];

	return newChannel;
}

QString M3UHandler::getName()
{
	return name;
}
