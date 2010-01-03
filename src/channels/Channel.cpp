#include "Channel.h"

Channel::Channel(QString name, int num, bool cat)
{
	_name = name;
	_num = num;
	category = cat;
}

Channel::~Channel() {

}

void Channel::setNum(int num) {
	_num = num;
}
void Channel::setName(QString xmlName) {
	_name = xmlName;
}
void Channel::setEpg(QString xmlEpg) {
	_epg = xmlEpg;
}
void Channel::setLanguage(QString xmlLanguage) {
	_lang = xmlLanguage;
}
void Channel::setUrl(QString xmlUrl) {
	_url = xmlUrl;
}
void Channel::setCategoryList(QStringList xmlCategoryList) {
	_categoryList = xmlCategoryList;
}

int Channel::num()
{
	return _num;
}
QString Channel::numToString()
{
	QString num;
	num.setNum(_num);
	return num;
}
QString Channel::name() {
	return _name;
}
bool Channel::isCategory() {
	return category;
}
QString Channel::language() {
	return _lang;
}
QString Channel::url() {
	return _url;
}
QString Channel::epg() {
	return _epg;
}
QStringList Channel::categoryList() {
	return _categoryList;
}
