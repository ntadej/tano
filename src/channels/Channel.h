#ifndef TANO_CHANNEL_H_
#define TANO_CHANNEL_H_

#include <QString>
#include <QStringList>

class Channel {
public:
	Channel(QString name, int num, bool cat);
	virtual ~Channel();

	void setNum(const int &num);
	void setName(const QString &xmlName);
	void setEpg(const QString &xmlEpg);
	void setLanguage(const QString &xmlLanguage);
	void setUrl(const QString &xmlUrl);
	void setCategoryList(const QStringList &xmlCategoryList);

	QString numToString();

	int num() const {return _num;};
	QString name() const {return _name;};
	bool isCategory() const {return _category;};
	QString language() const {return _lang;};
	QString url() const {return _url;};
	QString epg() const {return _epg;};
	QStringList categoryList() const {return _categoryList;};

private:
	int _num;
	QString _name;
	QString _lang;
	QString _url;
	QString _epg;
	QStringList _categoryList;
	bool _category;
};

#endif // TANO_CHANNEL_H_
