#ifndef LICENSE_H_
#define LICENSE_H_

#include <QTextBrowser>

class License : public QTextBrowser
{
    Q_OBJECT

public:
	License(QWidget *parent = 0);
	~License();
};

#endif /* LICENSE_H_ */
