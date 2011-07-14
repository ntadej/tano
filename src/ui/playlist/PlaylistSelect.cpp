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

#include <QtCore/QFile>
#include <QtXml/QXmlInputSource>
#include <QtXml/QXmlSimpleReader>

#include "container/playlist/PlaylistInfo.h"
#include "core/Common.h"
#include "core/Settings.h"
#include "xml/PlaylistListHandler.h"

#include "PlaylistSelect.h"
#include "ui_PlaylistSelect.h"

PlaylistSelect::PlaylistSelect(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::PlaylistSelect),
	_codec(QTextCodec::codecForName("UTF-8")),
	_playlist(Settings::DEFAULT_PLAYLIST)
{
	ui->setupUi(this);

	connect(ui->countryBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(list(QString)));
	connect(ui->playlistBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(listP(QString)));
}

PlaylistSelect::~PlaylistSelect()
{
	delete ui;
}

void PlaylistSelect::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void PlaylistSelect::list(const QString &id)
{
	ui->playlistBox->clear();
	ui->playlistBox->setEnabled(false);

	if(id == tr("Select"))
		return;

	ui->playlistBox->addItem(tr("Select"));
	for(int i = 0; i < _playlistList.size(); i++) {
		if(_playlistList[i].country() == id) {
			ui->playlistBox->addItem(_playlistList[i].title());
		}
	}
	ui->playlistBox->setEnabled(true);
}

void PlaylistSelect::listP(const QString &id)
{
	for(int i = 0; i < _playlistList.size(); i++) {
		if(_playlistList[i].title() == id) {
			_playlist = _playlistList[i].path();
			break;
		}
	}
}

void PlaylistSelect::open(const QString &list)
{
	readList(Tano::locateResource(list));
}

void PlaylistSelect::readList(const QString &list)
{
	QFile file(list);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString string = _codec->toUnicode(file.readAll());

	PlaylistListHandler *handler = new PlaylistListHandler();
	QXmlSimpleReader reader;
	reader.setContentHandler(handler);
	reader.setErrorHandler(handler);

	QXmlInputSource xmlInputSource;
	xmlInputSource.setData(string);
	if (!reader.parse(xmlInputSource))
		return;

	_playlistCountries = handler->playlistCountries();
	_playlistList = handler->playlistList();

	delete handler;

	ui->countryBox->clear();
	ui->countryBox->addItem(tr("Select"));
	ui->countryBox->addItems(_playlistCountries);
}

bool PlaylistSelect::setPlaylist(const QString &playlist) const
{
	bool ok = false;
	for(int i = 0; i < _playlistList.size(); i++) {
		if(_playlistList[i].path() == playlist) {
			for(int k = 0; k < ui->countryBox->count(); k++) {
				if(ui->countryBox->itemText(k) == _playlistList[i].country()) {
					ui->countryBox->setCurrentIndex(k);
				}
			}
			for(int k = 0; k < ui->playlistBox->count(); k++) {
				if(ui->playlistBox->itemText(k) == _playlistList[i].title()) {
					ui->playlistBox->setCurrentIndex(k);
				}
			}
			ok = true;
			break;
		}
	}

	return ok;
}
