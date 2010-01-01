#ifndef TANO_CHANNEL_H_
#define TANO_CHANNEL_H_
#include <QString>
#include <QStringList>

class Channel {
public:
	Channel(QString name, int num, bool cat);
	virtual ~Channel();

	void setNum(int num);
	void setName(QString xmlName);
	void setEpg(QString xmlEpg);
	void setLanguage(QString xmlLanguage);
	void setUrl(QString xmlUrl);
	void setCategoryList(QStringList xmlCategoryList);

	int num();
	QString numToString();
	QString name();
	bool isCategory();
	QString language();
	QString url();
	QString epg();
	QStringList categoryList();

private:
	int _num;
	QString _name;
	QString _lang;
	QString _url;
	QString _epg;
	QStringList _categoryList;
	bool category;
};

#endif // TANO_CHANNEL_H_
