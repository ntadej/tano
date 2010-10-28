#ifndef TANOQML_MAINQML_H_
#define TANOQML_MAINQML_H_

#include <QtDeclarative/QDeclarativeView>

class MainQml
{
public:
	MainQml();
	~MainQml();

	void show();

private:
	QDeclarativeView _view;
};

#endif // TANOQML_MAINQML_H_
