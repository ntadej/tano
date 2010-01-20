#include "Common.h"
#include "Updates.h"

Updates::Updates() {
	codec = QTextCodec::codecForName("UTF-8");
	setHost("tano.sourceforge.net");
}

Updates::~Updates() {

}

void Updates::getUpdates()
{
	get("/version.php");
	connect(this, SIGNAL(done(bool)), this, SLOT(updatesPrint()));
}

void Updates::updatesPrint()
{
	QByteArray httpResponse = readAll();

	QString string = codec->toUnicode(httpResponse);
	string = string.replace(" ","");
	string = string.replace("\n","");

	disconnect(this, SIGNAL(done(bool)), this, SLOT(updatesPrint()));

	processUpdates(string);
}

void Updates::processUpdates(QString v)
{
	QStringList updatesList;

	if(Common::version() != v) {
		if(Common::version().contains("svn"))
			updatesList << "svn" << v << Common::version();
		else
			updatesList << "update" << v;
	} else {
		updatesList << "latest";
	}

	emit updatesDone(updatesList);
}
