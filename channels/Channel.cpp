#include "Channel.h"

Channel::Channel(QString name, bool cat)
{
	_name = name;
	category = cat;
}

Channel::~Channel() {

}

void Channel::setName(QString xmlName) {
	_name = xmlName;
}
void Channel::setLongName(QString xmlLongName) {
	_long = xmlLongName;
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

QString Channel::name() {
	return _name;
}

QString Channel::longName() {
	return _long;
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
