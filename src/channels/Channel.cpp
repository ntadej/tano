#include "Channel.h"

Channel::Channel(QString name, int num, bool cat)
	: _name(name), _num(num), _category(cat)
{

}

Channel::~Channel() {

}

void Channel::setNum(const int &num) {
	_num = num;
}
void Channel::setName(const QString &xmlName) {
	_name = xmlName;
}
void Channel::setEpg(const QString &xmlEpg) {
	_epg = xmlEpg;
}
void Channel::setLanguage(const QString &xmlLanguage) {
	_lang = xmlLanguage;
}
void Channel::setUrl(const QString &xmlUrl) {
	_url = xmlUrl;
}
void Channel::setCategoryList(const QStringList &xmlCategoryList) {
	_categoryList = xmlCategoryList;
}

QString Channel::numToString()
{
	QString num;
	num.setNum(_num);
	return num;
}
