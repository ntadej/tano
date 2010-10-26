#include <QtCore/QDebug>
#include <QtDeclarative/qdeclarative.h>

#include "MainQml.h"

MainQml::MainQml()
{
	//qmlRegisterInterface<QGraphicsLayoutItem>("QGraphicsLayoutItem");
	//qmlRegisterType<GraphicsGridLayoutObject>("GridLayouts", 4, 7, "GraphicsGridLayout");

	view.setSource(QUrl("qrc:/qml/Main.qml"));
}

MainQml::~MainQml() { }

void MainQml::show()
{
	view.show();
}
