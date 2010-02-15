#include <QtCore/QDate>

#include <QVlc/VlcInstance.h>

#include "About.h"
#include "../Ver.h"

About::About(QWidget *parent, QString version)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.labelVersion->setText("<h2>"+version+"</h2>");
	ui.labelCopy->setText(ui.labelCopy->text().replace("%1",QDate().currentDate().toString("yyyy")));
	ui.labelLicense->setText(ui.labelLicense->text().replace("GNU GPL","<a href='#'>GNU GPL</a>"));
	ui.labelLinks->setText(ui.labelLinks->text().replace("http://tanoplayer.co.cc","<a href='http://tanoplayer.co.cc'>http://tanoplayer.co.cc</a>"));
	ui.labelLinks->setText(ui.labelLinks->text().replace("info@tanoplayer.co.cc","<a href='info@tanoplayer.co.cc'>info@tanoplayer.co.cc</a>"));

	ui.labelVlc->setText("<h3>libVLC "+VlcInstance::version().split(" ")[0]+"</h3>");
	ui.labelQt->setText("<h3>Qt "+QtVersion()+"</h3>");

	connect(ui.buttonLicense, SIGNAL(clicked()), this, SLOT(license()));
	connect(ui.labelLicense, SIGNAL(linkActivated(QString)), this, SLOT(license()));
}

About::~About()
{

}

void About::license()
{
	if(ui.stackedWidget->currentIndex() == 1)
		ui.stackedWidget->setCurrentIndex(0);
	else
		ui.stackedWidget->setCurrentIndex(1);
}
