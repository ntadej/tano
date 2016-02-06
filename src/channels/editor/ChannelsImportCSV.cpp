/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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

#include "ChannelsImportCSV.h"
#include "ui_ChannelsImportCSV.h"

ChannelsImportCSV::ChannelsImportCSV(QWidget *parent)
	: QDialog(parent),
    ui(new Ui::ChannelsImportCSV),
	_proceed(false)
{
	ui->setupUi(this);

	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));
}

ChannelsImportCSV::~ChannelsImportCSV()
{
	delete ui;
}

void ChannelsImportCSV::changeEvent(QEvent *e)
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

void ChannelsImportCSV::action(QAbstractButton *button)
{
	switch(ui->buttonBox->standardButton(button)) {
		case QDialogButtonBox::Ok:
			_proceed = true;
			close();
			break;
		case QDialogButtonBox::Cancel:
			close();
			break;
		default:
			break;
	}
}

QList<int> ChannelsImportCSV::columns() const
{
	QList<int> list;

	int number = 0;
	int channel = 1;
	int url = 2;
	int categories = 3;
	int language = 4;
	int epg = 5;

	for(int i = 0; i < ui->columns->count(); i++) {
		if(ui->columns->item(i)->text() == tr("Number")) {
			number = i;
		} else if(ui->columns->item(i)->text() == tr("Channel")) {
			channel = i;
		} else if(ui->columns->item(i)->text() == tr("URL")) {
			url = i;
		} else if(ui->columns->item(i)->text() == tr("Categories")) {
			categories = i;
		} else if(ui->columns->item(i)->text() == tr("Language")) {
			language = i;
		} else if(ui->columns->item(i)->text() == tr("EPG ID")) {
			epg = i;
		}
	}

	list << number << channel << url << categories << language << epg;
	return list;
}

bool ChannelsImportCSV::header() const
{
	return ui->radioFirstHeader->isChecked();
}

QString ChannelsImportCSV::separator() const
{
	return ui->editSeparator->text();
}
