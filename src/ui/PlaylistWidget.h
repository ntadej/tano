#ifndef TANO_PLAYLISTWIDGET_H_
#define TANO_PLAYLISTWIDGET_H_

#include <QtGui/QWidget>

#include "../channels/Channel.h"
#include "../xml/M3UHandler.h"

#include <ui_PlaylistWidget.h>

class PlaylistWidget : public QWidget
{
Q_OBJECT

public:
	PlaylistWidget(QWidget *parent = 0);
	~PlaylistWidget();

	void clear();
	void open(const QString &file);
	QString name();
	QString fileName();
	QList<int> nums();
	QTreeWidget *treeWidget();

public slots:
	Channel *channelRead(QTreeWidgetItem* clickedChannel);
	Channel *channelRead(const int &clickedChannel);

signals:
	void itemClicked(QTreeWidgetItem*, const int);

private slots:
	void processCategories(const QString &cat);
	void processSearch(const QString &search);

private:
	Ui::PlaylistWidget ui;

	M3UHandler *handler;
	QString _fileName;
};

#endif // TANO_PLAYLISTWIDGET_H_
