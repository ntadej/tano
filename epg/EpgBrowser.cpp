#include <QUrl>
#include <QWebFrame>

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

	connect(ui.actionHome, SIGNAL(triggered()), this, SLOT(home()));
	connect(ui.epgView, SIGNAL(loadProgress(int)), this, SLOT(done(int)));
	connect(ui.epgView, SIGNAL(loadProgress(int)), bar, SLOT(setValue(int)));
	connect(ui.epgView, SIGNAL(loadStarted()), this, SLOT(stopStatus()));
	connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(stopStatus()));
	connect(ui.epgView, SIGNAL(loadFinished(bool)), this, SLOT(stopStatusT(bool)));
}

EpgBrowser::~EpgBrowser()
{

}

void EpgBrowser::open(QString link) {
	ui.epgView->load(QUrl(link));
	this->show();
}

void EpgBrowser::home()
{
	ui.epgView->load(QUrl("http://www.siol.net/tv-spored.aspx"));
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
		ui.statusbar->showMessage(tr("Loading"));
		epg();
		break;
	case 100:
		ui.statusbar->showMessage(tr("Done"),5000);
		epg();
		break;
	default:
		epg();
		break;
	}
}

QString EpgBrowser::evalJS(const QString &js)
{
	QWebFrame *frame = ui.epgView->page()->mainFrame();
	return frame->evaluateJavaScript(js).toString();
}

void EpgBrowser::epg()
{
	evalJS("document.getElementById('header').style.background = 'transparent';");
	evalJS("document.getElementsByClassName('holder')[0].style.background = 'transparent';");

	evalJS("document.getElementsByClassName('nav_corpo')[0].parentNode.removeChild(document.getElementsByClassName('nav_corpo')[0]);");
	evalJS("document.getElementsByClassName('nav_tools')[0].parentNode.removeChild(document.getElementsByClassName('nav_tools')[0]);");
	evalJS("document.getElementsByClassName('qjump')[0].parentNode.removeChild(document.getElementsByClassName('qjump')[0]);");
	evalJS("document.getElementById('nav_primary').parentNode.removeChild(document.getElementById('nav_primary'));");
	evalJS("document.getElementById('f_search').parentNode.removeChild(document.getElementById('f_search'));");
	evalJS("document.getElementById('footer').innerHTML = '<p>&copy 2008-2009 Tano</p>';");
	evalJS("document.getElementsByClassName('gocorpo')[0].parentNode.removeChild(document.getElementsByClassName('gocorpo')[0]);");
	evalJS("document.getElementsByClassName('tools')[0].parentNode.removeChild(document.getElementsByClassName('tools')[0]);");

	evalJS("document.getElementsByClassName('container')[0].innerHTML = '<h1><a href=\"http://www.siol.net/tv-spored.aspx\"><img src=\"http://tano.sourceforge.net/schedule.png\" alt=\"Tano\"/></a><span style=font-size:40px>TV SPORED</span></h1>';");
}
