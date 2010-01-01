#ifndef TANO_EDITPLAYLIST_H
#define TANO_EDITPLAYLIST_H

#include <QMainWindow>
#include <QMenu>
#include "ui_EditPlaylist.h"

#include "../xml/M3UHandler.h"
#include "../xml/M3UGenerator.h"

class EditPlaylist : public QMainWindow
{
    Q_OBJECT

public:
    EditPlaylist(QWidget *parent = 0, QString fileName = "");
    ~EditPlaylist();

    void setFile(QString file);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void deleteItem();
	void addItemCategory();
	void addItemSubCategory();
	void addItemChannel();

	void open();
	void save();

	void menuOpen();
	void exit();

private:
	void treeStyle();

	bool closeEnabled;

    Ui::EditPlaylist ui;

    M3UHandler *load;
    M3UGenerator *generator;

    QIcon categoryIcon;
    QIcon channelIcon;

    QString fileN;
    QString fileName;

    QMenu *add;
};

#endif // TANO_EDITPLAYLIST_H
