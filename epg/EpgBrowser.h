#ifndef EPGBROWSER_H
#define EPGBROWSER_H

#include <QtGui/QWidget>
#include "../ui_EpgBrowser.h"

class EpgBrowser : public QWidget
{
    Q_OBJECT

public:
    EpgBrowser(QWidget *parent = 0);
    ~EpgBrowser();

public slots:
	void open(QString link);
	void help();

private slots:
	void go();
	void home();
	void epg();
	void changeText(QUrl url);

	void stopStatus();
	void stopStatusT(bool status);

private:
    Ui::EpgBrowserClass ui;
};

#endif // EPGBROWSER_H
