#include <QUrl>
#include <QDebug>

#include "EpgBrowser.h"

EpgBrowser::EpgBrowser(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.buttonGo, SIGNAL(clicked()), this, SLOT(go()));
	connect(ui.actionHome, SIGNAL(triggered()), this, SLOT(home()));
	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(help()));
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

void EpgBrowser::open(QString link) {
	ui.addressLine->setText(link);
	go();
	this->show();
}

void EpgBrowser::home()
{
	ui.addressLine->setText("siol-tv.pfusion.co.cc");
	go();
}

void EpgBrowser::help()
{
	ui.addressLine->setText("http://apps.sourceforge.net/mediawiki/tano/index.php?title=Help:Contents");
	go();
	this->show();
}

void EpgBrowser::changeText(QUrl url)
{
	ui.addressLine->setText(url.toString());
}
