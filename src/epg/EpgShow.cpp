#include <QResource>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QDebug>

#include "EpgShow.h"

EpgShow::EpgShow(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	loader = new EpgLoader();
	http = new QHttp(this);

	connect(loader, SIGNAL(epgDone(QStringList)), this, SLOT(display(QStringList)));

	connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
}

EpgShow::~EpgShow()
{

}

void EpgShow::open(QString url)
{
	setWindowTitle("");
	ui.labelName->setText("");
	ui.labelTime->setText("");
	ui.labelInfo->setText("");
	ui.labelDescription->setText("");
	ui.labelActors->setText("");
	ui.labelPhoto->setPixmap(QPixmap(":/icons/images/image.png"));
	this->show();
	loader->getEpg(false,url);
}

void EpgShow::display(QStringList list)
{
	setWindowTitle(list.at(0));
	ui.labelName->setText("<h1>" + list.at(0) + "</h1>");
	ui.labelTime->setText("<h2>" + list.at(1) + "</h2>");
	ui.labelInfo->setText("<h3>" + list.at(2) + "</h3>");
	ui.labelDescription->setText("\n" + list.at(3));
	ui.labelActors->setText("\n" + list.at(4));

	downloadFile(list.at(5));
}

void EpgShow::downloadFile(QString u)
{
	if(u == "NI")
		return;

    QUrl url(u);
    QFileInfo fileInfo(url.path());
    fileName = fileInfo.fileName();

    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }

    file = new QFile(QDir::tempPath() + "/" + fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        delete file;
        file = 0;
        return;
    }

    QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
    http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

    if (!url.userName().isEmpty())
        http->setUser(url.userName(), url.password());

    httpRequestAborted = false;
    QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
    if (path.isEmpty())
        path = "/";

    qDebug() << "Downloading..." << file->fileName();
    httpGetId = http->get(path, file);
}

void EpgShow::httpRequestFinished(int requestId, bool error)
{
    if (requestId != httpGetId)
        return;
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        return;
    }

    if (requestId != httpGetId)
    	return;
    file->close();

    if (!error) {
    	ui.labelPhoto->setPixmap(QPixmap(file->fileName()));
    }

    delete file;
    file = 0;
}
