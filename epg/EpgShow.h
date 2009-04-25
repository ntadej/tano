#ifndef EPGSHOW_H
#define EPGSHOW_H

#include <QtGui/QWidget>
#include <QFile>

#include "ui_EpgShow.h"
#include "EpgLoader.h"

class EpgShow : public QWidget
{
    Q_OBJECT

public:
    EpgShow(QWidget *parent = 0);
    ~EpgShow();

public slots:
	void open(QString url);

private slots:
	void display(QStringList list);
	void downloadFile(QString u);
	void httpRequestFinished(int requestId, bool error);

private:
    Ui::EpgShow ui;

    EpgLoader *loader;

    QString fileName;
    QHttp *http;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;
};

#endif // EPGSHOW_H
