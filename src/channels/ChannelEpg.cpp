#include "ChannelEpg.h"

ChannelEpg::ChannelEpg(QString wtime, QString wurl, QString wtitle) {
	_time = wtime;
	_url = wurl;
	_title = wtitle;
}

ChannelEpg::~ChannelEpg() {

}

QString ChannelEpg::time() {
	return _url;
}

QString ChannelEpg::url() {
	return _url;
}

QString ChannelEpg::title() {
	return _url;
}
