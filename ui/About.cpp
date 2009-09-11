#include <vlc/vlc.h>

#include "About.h"

About::About(QWidget *parent, QString version)
    : QDialog(parent)
{
	ui.setupUi(this);

	ui.labelVersion->setText(ui.labelVersion->text().replace("><",">"+version+"<"));
	ui.labelCopy->setText(ui.labelCopy->text().replace("http://tano.sourceforge.net","<a href='http://tano.sourceforge.net'>http://tano.sourceforge.net</a>"));
	ui.labelCopy->setText(ui.labelCopy->text().replace("ntadej@users.sourceforge.net","<a href='mailto:ntadej@users.sourceforge.net'>ntadej@users.sourceforge.net</a>"));

#ifdef TANO_QT_VERSION
	qt = QString(TANO_QT_VERSION);
#else
	qt = "Unknown version";
#endif

	ui.labelVlc->setText(tr("Powered by Videolan VLC")+" "+QString(libvlc_get_version()));
	ui.labelCopyrights->setText(ui.labelCopyrights->text().replace("friptv","<a href=http://friptv.sourceforge.net>friptv</a>"));

	ui.labelQtVersion->setText(ui.labelQtVersion->text().replace("0.0.0",qt));
	ui.labelQtLicenceLink->setText(ui.labelQtLicenceLink->text().replace("www.qtsoftware.com/about/open-source-business-model","<a href='http://www.qtsoftware.com/about/open-source-business-model'>www.qtsoftware.com/about/open-source-business-model</a>"));
	ui.labelQtLink->setText(ui.labelQtLink->text().replace("www.qtsoftware.com/products/appdev","<a href='http://www.qtsoftware.com/products/appdev'>www.qtsoftware.com/products/appdev</a>"));
}

About::~About()
{

}
