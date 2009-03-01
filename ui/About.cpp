#include "About.h"

About::About(QWidget *parent, QString version, QString build)
    : QDialog(parent)
{
	ui.setupUi(this);

	ui.labelVersion->setText(ui.labelVersion->text().replace("><",">"+version+" ("+build+")<"));
	ui.labelLink->setText(ui.labelLink->text().replace("http://tano.pfusion.co.cc","<a href='http://tano.pfusion.co.cc'>http://tano.pfusion.co.cc</a>"));

#ifdef Q_WS_WIN
	ui.labelBackend->setText("VLC Backend");
#else
	ui.labelBackend->setText("Linux Backend");
#endif

#ifdef TANO_QT_VERSION
	qt = QString(TANO_QT_VERSION);
#else
	qt = "Unknown version";
#endif

	ui.labelQtVersion->setText(ui.labelQtVersion->text().replace("0.0.0",qt));
	ui.labelQtLicenceLink->setText(ui.labelQtLicenceLink->text().replace("www.qtsoftware.com/about/open-source-business-model","<a href='http://www.qtsoftware.com/about/open-source-business-model'>www.qtsoftware.com/about/open-source-business-model</a>"));
	ui.labelQtLink->setText(ui.labelQtLink->text().replace("www.qtsoftware.com/products/appdev","<a href='http://www.qtsoftware.com/products/appdev'>www.qtsoftware.com/products/appdev</a>"));

}

About::~About()
{

}
