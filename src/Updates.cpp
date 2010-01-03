#include "Updates.h"

#include <QtDebug>

Updates::Updates() {

	qDebug() << "Updates Success!";

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

	qDebug() << string;

	disconnect(this, SIGNAL(done(bool)), this, SLOT(updatesPrint()));

	emit updatesDone(string);
}
