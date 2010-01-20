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
	void open(QString file);
	QString name();
	QString fileName();
	QList<int> nums();

public slots:
	Channel *channelRead(QTreeWidgetItem* clickedChannel);
	Channel *channelReadNum(int clickedChannel);

signals:
	void itemClicked(QTreeWidgetItem*, int);

private slots:
	void processCategories(QString cat);
	void processSearch(QString search);

private:
    Ui::PlaylistWidget ui;

	M3UHandler *handler;
	QString _fileName;
};

#endif // TANO_PLAYLISTWIDGET_H_
