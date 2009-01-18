#ifndef EPGBROWSER_H
#define EPGBROWSER_H

#include <QtGui/QDialog>
#include "../ui_EpgBrowser.h"

class EpgBrowser : public QDialog
{
    Q_OBJECT

public:
    EpgBrowser(QWidget *parent = 0, QString link = "http://siol-tv.pfusion.co.cc");
    ~EpgBrowser();

private slots:
	void go();
	void home();
	void changeText(QUrl url);

private:
    Ui::EpgBrowserClass ui;
};

#endif // EPGBROWSER_H
