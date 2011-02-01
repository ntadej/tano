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
#include <QtGui/QDialogButtonBox>
#include <QtXml/QXmlInputSource>
#include <QtXml/QXmlSimpleReader>
#include <QDebug>

#include "core/GetFile.h"
#include "xml/WebPlaylistHandler.h"

#include "PlaylistImportWeb.h"
#include "ui_PlaylistImportWeb.h"

PlaylistImportWeb::PlaylistImportWeb(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::PlaylistImportWeb),
	_codec(QTextCodec::codecForName("UTF-8"))
{
	ui->setupUi(this);

	_handler = new WebPlaylistHandler();
	_file = new GetFile(this);

	connect(_file, SIGNAL(file(QString)), this, SLOT(readList(QString)));

	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));
	connect(ui->countryBox, SIGNAL(currentIndexChanged(int)), this, SLOT(getList(int)));
}

PlaylistImportWeb::~PlaylistImportWeb()
{
	delete ui;
	delete _handler;
	delete _file;
}

void PlaylistImportWeb::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void PlaylistImportWeb::action(QAbstractButton *button)
{
	switch(ui->buttonBox->standardButton(button)) {
		case QDialogButtonBox::Save:
			_playlist = "";
			close();
			break;
		case QDialogButtonBox::Cancel:
			_playlist = "";
			close();
			break;
		default:
			break;
	}
}

void PlaylistImportWeb::getList(const int &id)
{
	qDebug() << id;
	if(id == 1)
		_file->getFile("http://sloiptv.tano.si/playlists/channel-lists-v2.xml?attredirects=0");
	else
		return;
}

void PlaylistImportWeb::readList(const QString &list)
{
	QFile file(list);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString string = _codec->toUnicode(_file->readAll());
	qDebug() << list << string;

	QXmlSimpleReader reader;
	reader.setContentHandler(_handler);
	reader.setErrorHandler(_handler);

	QXmlInputSource xmlInputSource;
	xmlInputSource.setData(string);
	if (!reader.parse(xmlInputSource))
		return;

	_playlistName = _handler->playlistName();
	_playlistUrl = _handler->playlistUrl();

	ui->playlistBox->clear();
	ui->playlistBox->addItem(tr("Select"));
	ui->playlistBox->addItems(_playlistName);
	ui->playlistBox->setEnabled(true);
}
