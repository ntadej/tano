#include <QUrl>

#include "EpgBrowser.h"

EpgBrowser::EpgBrowser(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);

	ui.actionStop->setDisabled(true);

	bar = new QProgressBar(this);
	bar->setRange(0,100);
	bar->setValue(0);
	bar->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred));
	bar->setMaximumWidth(200);
	bar->setMinimumWidth(200);

	ui.statusbar->insertPermanentWidget(0,bar);

	connect(ui.buttonGo, SIGNAL(clicked()), this, SLOT(go()));
	connect(ui.actionHome, SIGNAL(triggered()), this, SLOT(home()));
	connect(ui.epgView, SIGNAL(urlChanged(QUrl)), this, SLOT(changeText(QUrl)));

	connect(ui.epgView, SIGNAL(loadProgress(int)), this, SLOT(done(int)));
	connect(ui.epgView, SIGNAL(loadProgress(int)), bar, SLOT(setValue(int)));

	connect(ui.epgView, SIGNAL(loadStarted()), this, SLOT(stopStatus()));
	connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(stopStatus()));
	connect(ui.epgView, SIGNAL(loadFinished(bool)), this, SLOT(stopStatusT(bool)));
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
	ui.addressLine->setText("http://tano.sf.net");
	go();
}

void EpgBrowser::changeText(QUrl url)
{
	ui.addressLine->setText(url.toString());
}

void EpgBrowser::stopStatusT(bool status)
{
    if(status == true)
    	stopStatus();
}

void EpgBrowser::stopStatus()
{
    if(ui.actionStop->isEnabled() != true)
    	ui.actionStop->setEnabled(true);
    else
    	ui.actionStop->setDisabled(true);

    if(ui.actionReload->isEnabled() != true)
    	ui.actionReload->setEnabled(true);
    else
    	ui.actionReload->setDisabled(true);
}

void EpgBrowser::done(int value)
{
	switch (value) {
	case 10:
		ui.statusbar->showMessage(tr("Loading")+" "+ui.addressLine->text());
		break;
	case 100:
		ui.statusbar->showMessage(tr("Done"),5000);
		break;
	default:
		break;
	}
}
