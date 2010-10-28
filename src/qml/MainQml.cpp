#include <QtCore/QDebug>
#include <QtDeclarative/qdeclarative.h>

#include "MainQml.h"
#include "Video.h"

MainQml::MainQml()
{
	//qmlRegisterInterface<QGraphicsLayoutItem>("QGraphicsLayoutItem");
	qmlRegisterType<VideoDisplay>("VideoDisplay", 0, 1, "VideoDisplay");

	_view.setSource(QUrl("qrc:/qml/Main.qml"));
}

MainQml::~MainQml() { }

void MainQml::show()
{
	_view.show();
}
