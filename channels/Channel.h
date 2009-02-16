#ifndef CHANNEL_H_
#define CHANNEL_H_
#include <QString>

class Channel {
public:
	Channel(QString name, int num, bool cat);
	virtual ~Channel();

	void setNum(int num);
	void setName(QString xmlName);
	void setLongName(QString xmlLongName);
	void setEpg(QString xmlEpg);
	void setLanguage(QString xmlLanguage);
	void setUrl(QString xmlUrl);

	int num();
	QString numToString();
	QString name();
	QString longName();
	bool isCategory();
	QString language();
	QString url();
	QString epg();

private:
	int _num;
	QString _name;
	QString _long;
	QString _lang;
	QString _url;
	QString _epg;
	bool category;
};

#endif
