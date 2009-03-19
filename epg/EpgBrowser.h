#ifndef EPGBROWSER_H
#define EPGBROWSER_H

#include <QtGui/QMainWindow>
#include <QProgressBar>
#include "../ui_EpgBrowser.h"

class EpgBrowser : public QMainWindow
{
    Q_OBJECT

public:
    EpgBrowser(QWidget *parent = 0);
    ~EpgBrowser();

public slots:
	void open(QString link);

private slots:
	void go();
	void home();
	void epg();
	void changeText(QUrl url);

	void done(int value);

	void stopStatus();
	void stopStatusT(bool status);

private:
    Ui::EpgBrowserClass ui;

    QProgressBar *bar;
};

#endif // EPGBROWSER_H
