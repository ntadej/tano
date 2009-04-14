#include "About.h"

About::About(QWidget *parent, QString version, QString build)
    : QDialog(parent)
{
	ui.setupUi(this);

	ui.labelVersion->setText(ui.labelVersion->text().replace("><",">"+version+" ("+build+")<"));
	ui.labelCopy->setText(ui.labelCopy->text().replace("http://tano.sourceforge.net","<a href='http://tano.sourceforge.net'>http://tano.sourceforge.net</a>"));
	ui.labelCopy->setText(ui.labelCopy->text().replace("ntadej@users.sourceforge.net","<a href='mailto:ntadej@users.sourceforge.net'>ntadej@users.sourceforge.net</a>"));

#ifdef Q_WS_WIN
	ui.labelBackendInfo->setText(ui.labelBackendInfo->text().replace("Backend","VLC Backend"));
#else
	ui.labelBackendInfo->setText(ui.labelBackendInfo->text().replace("Backend","Linux Backend"));
#endif

#ifdef TANO_QT_VERSION
	qt = QString(TANO_QT_VERSION);
#else
	qt = "Unknown version";
#endif

	ui.labelBackendInfo->setText(ui.labelBackendInfo->text().replace("friptv","<a href=http://friptv.sourceforge.net>friptv</a>"));

	ui.labelQtVersion->setText(ui.labelQtVersion->text().replace("0.0.0",qt));
	ui.labelQtLicenceLink->setText(ui.labelQtLicenceLink->text().replace("www.qtsoftware.com/about/open-source-business-model","<a href='http://www.qtsoftware.com/about/open-source-business-model'>www.qtsoftware.com/about/open-source-business-model</a>"));
	ui.labelQtLink->setText(ui.labelQtLink->text().replace("www.qtsoftware.com/products/appdev","<a href='http://www.qtsoftware.com/products/appdev'>www.qtsoftware.com/products/appdev</a>"));

}

About::~About()
{

}
