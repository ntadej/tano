#include <QVlc/VlcInstance.h>

#include <QDate>

#include "About.h"
#include "../Ver.h"

About::About(QWidget *parent, QString version)
    : QDialog(parent)
{
	ui.setupUi(this);

	ui.labelVersion->setText(ui.labelVersion->text().replace("><",">"+version+"<"));
	ui.labelCopy->setText(ui.labelCopy->text().replace("%1",QDate().currentDate().toString("yyyy")));
	ui.labelCopy->setText(ui.labelCopy->text().replace("http://tanoplayer.co.cc","<a href='http://tanoplayer.co.cc'>http://tanoplayer.co.cc</a>"));
	ui.labelCopy->setText(ui.labelCopy->text().replace("info@tanoplayer.co.cc","<a href='info@tanoplayer.co.cc'>info@tanoplayer.co.cc</a>"));

	ui.labelVlc->setText("<h3>Videolan VLC "+VlcInstance::version()+"</h3>");
	ui.labelRecorder->setText(ui.labelRecorder->text().replace("friptv","<a href=http://friptv.sourceforge.net>friptv</a>"));

	ui.labelQtVersion->setText(ui.labelQtVersion->text().replace("0.0.0",QtVersion()));
	ui.labelQtLicenceLink->setText(ui.labelQtLicenceLink->text().replace("http://qt.nokia.com/products/licensing","<a href='http://qt.nokia.com/products/licensing'>http://qt.nokia.com/products/licensing</a>"));
	ui.labelQtLink->setText(ui.labelQtLink->text().replace("http://qt.nokia.com/products/appdev","<a href='http://qt.nokia.com/products/appdev'>http://qt.nokia.com/products/appdev</a>"));
}

About::~About()
{

}
