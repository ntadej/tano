#include <QUrl>
#include <QDebug>

#include "EpgBrowser.h"

EpgBrowser::EpgBrowser(QWidget *parent, QString link)
    : QDialog(parent)
{
	ui.setupUi(this);
	ui.addressLine->setText(link);
	go();

	connect(ui.buttonGo, SIGNAL(clicked()), this, SLOT(go()));
	connect(ui.actionHome, SIGNAL(triggered()), this, SLOT(home()));
	connect(ui.epgView, SIGNAL(urlChanged(QUrl)), this, SLOT(changeText(QUrl)));
}

EpgBrowser::~EpgBrowser()
{

}

void EpgBrowser::go()
{
	QString url = ui.addressLine->text();
	if (!url.contains("http://", Qt::CaseInsensitive)) {
		url.prepend("http://");
		ui.addressLine->setText(url);
	}
	ui.epgView->load(QUrl(ui.addressLine->text()));
}

void EpgBrowser::home()
{
	ui.addressLine->setText("siol-tv.pfusion.co.cc");
	go();
}

void EpgBrowser::changeText(QUrl url)
{
	ui.addressLine->setText(url.toString());
}
