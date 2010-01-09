#ifndef PLAYLISTWIDGET_H_
#define PLAYLISTWIDGET_H_

#include <QtGui/QWidget>

#include "ui_PlaylistWidget.h"

class PlaylistWidget : public QWidget
{
    Q_OBJECT

public:
	PlaylistWidget(QWidget *parent = 0);
	~PlaylistWidget();

	QTreeWidget* treeWidget();
	void clear();
	void setCategories(QStringList c);

signals:
	void itemClicked(QTreeWidgetItem*, int);

private slots:
	void processCategories(QString cat);
	void processSearch(QString search);

private:
    Ui::PlaylistWidget ui;

};

#endif /* PLAYLISTWIDGET_H_ */
