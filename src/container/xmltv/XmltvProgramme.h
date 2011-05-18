/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef TANO_XMLTVPROGRAMME_H_
#define TANO_XMLTVPROGRAMME_H_

#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtCore/QStringList>

class XmltvProgramme
{
public:
	XmltvProgramme(const QString &channel);
	~XmltvProgramme();

	QString channel() const { return _channel; }
	QString channelDisplayName() const { return _channelDisplayName; }
	void setChannelDisplayName(const QString &s) { _channelDisplayName = s; }
	QString title() const { return _title; }
	void setTitle(const QString &s) { _title = s; }
	QDateTime start() const { return _start; }
	void setStart(const QDateTime &d) { _start = d; }
	QDateTime stop() const { return _stop; }
	void setStop(const QDateTime &d) { _stop = d; }
	QString subTitle() const { return _subTitle; }
	void setSubTitle(const QString &s) { _subTitle = s; }
	QString desc() const { return _desc; }
	void setDesc(const QString &s) { _desc = s; }
	QDateTime date() const { return _date; }
	void setDate(const QDateTime &d) { _date = d; }
	QStringList category() const { return _category; }
	void addCategory(const QString &s) { _category << s; }

	// Credits
	QStringList director() const { return _director; }
	void addDirector(const QString &s) { _director << s; }
	QStringList actor() const { return _actor; }
	void addActor(const QString &s) { _actor << s; }
	QStringList writer() const { return _writer; }
	void addWriter(const QString &s) { _writer << s; }
	QStringList adapter() const { return _adapter; }
	void addAdapter(const QString &s) { _adapter << s; }
	QStringList producer() const { return _producer; }
	void addProducer(const QString &s) { _producer << s; }
	QStringList composer() const { return _composer; }
	void addComposer(const QString &s) { _composer << s; }
	QStringList editor() const { return _editor; }
	void addEditor(const QString &s) { _editor << s; }
	QStringList presenter() const { return _presenter; }
	void addPresenter(const QString &s) { _presenter << s; }
	QStringList commentator() const { return _commentator; }
	void addCommentator(const QString &s) { _commentator << s; }
	QStringList guest() const { return _guest; }
	void addGuest(const QString &s) { _guest << s; }

private:
	QString _channel;
	QString _channelDisplayName;
	QString _title;
	QDateTime _start;
	QDateTime _stop;
	QString _subTitle;
	QString _desc;
	QDateTime _date;
	QStringList _category;

	// Credits
	QStringList _director;
	QStringList _actor;
	QStringList _writer;
	QStringList _adapter;
	QStringList _producer;
	QStringList _composer;
	QStringList _editor;
	QStringList _presenter;
	QStringList _commentator;
	QStringList _guest;
};

#endif // TANO_XMLTVPROGRAMME_H_
