#ifndef EDITPLAYLIST_H
#define EDITPLAYLIST_H

#include <QtGui/QDialog>
#include "../ui_EditPlaylist.h"

#include "../xml/tanohandler.h"
#include "../xml/tanogenerator.h"

class EditPlaylist : public QDialog
{
    Q_OBJECT

public:
    EditPlaylist(QWidget *parent = 0, QString fileName = "");
    ~EditPlaylist();

private slots:
	void deleteItem();
	void addItemCategory();
	void addItemChannel();

	void save();

private:
	void treeStyle();

    Ui::EditPlaylistClass ui;

    TanoHandler *load;
    TanoGenerator *generator;

    QIcon categoryIcon;
    QIcon channelIcon;

    QString fileName;
};

#endif // EDITPLAYLIST_H
