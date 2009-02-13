#ifndef EDITPLAYLIST_H
#define EDITPLAYLIST_H

#include <QtGui/QWidget>
#include "../ui_EditPlaylist.h"

#include "../xml/tanohandler.h"
#include "../xml/tanogenerator.h"

class EditPlaylist : public QWidget
{
    Q_OBJECT

public:
    EditPlaylist(QWidget *parent = 0, QString fileName = "");
    ~EditPlaylist();

    void setFile(QString file);

private slots:
	void deleteItem();
	void addItemCategory();
	void addItemSubCategory();
	void addItemChannel();

	void open();
	void save();

private:
	void treeStyle();

    Ui::EditPlaylistClass ui;

    TanoHandler *load;
    TanoGenerator *generator;

    QIcon categoryIcon;
    QIcon channelIcon;

    QString fileN;
    QString fileName;
};

#endif // EDITPLAYLIST_H
