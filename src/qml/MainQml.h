#ifndef TANO_MAINQML_H_
#define TANO_MAINQML_H_

#include <QtDeclarative/QDeclarativeView>

class MainQml
{
public:
	MainQml();
	~MainQml();

	void show();

private:
	QDeclarativeView view;
};

#endif // TANO_MAINQML_H_
