#ifndef TANO_EDITPLAYLIST_H
#define TANO_EDITPLAYLIST_H

#include <QMainWindow>

#include <ui_EditPlaylist.h>

class EditPlaylist : public QMainWindow
{
    Q_OBJECT

public:
    EditPlaylist(QWidget *parent = 0);
    ~EditPlaylist();

    void setFile(QString file);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void deleteItem();
	void addItemChannel();

	void open();
	void save();
	void import();

	void exit();

private:
	bool closeEnabled;

    Ui::EditPlaylist ui;

    QIcon channelIcon;

    QString fileN;
    QString fileName;
};

#endif // TANO_EDITPLAYLIST_H
