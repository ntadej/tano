#ifndef TANO_LICENSE_H_
#define TANO_LICENSE_H_

#include <QTextBrowser>

class License : public QTextBrowser
{
    Q_OBJECT

public:
	License(QWidget *parent = 0);
	~License();
};

#endif // TANO_LICENSE_H_
