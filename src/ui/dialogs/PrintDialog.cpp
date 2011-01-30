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

#include <QtGui/QDialogButtonBox>

#include "PrintDialog.h"
#include "ui_PrintDialog.h"

PrintDialog::PrintDialog(const QString &name,
						 PlaylistWidget *playlist,
						 QWidget *parent)
	: QDialog(parent),
	ui(new Ui::PrintDialog),
	_name(name),
	_playlist(playlist)
{
	ui->setupUi(this);

	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(action(QAbstractButton *)));

	_print = new Print();
}

PrintDialog::~PrintDialog()
{
	delete ui;
	delete _print;
}

void PrintDialog::changeEvent(QEvent *e)
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

void PrintDialog::action(QAbstractButton *button)
{
	switch(ui->buttonBox->standardButton(button)) {
		case QDialogButtonBox::Ok:
			hide();
			print();
			break;
		case QDialogButtonBox::Cancel:
			close();
			break;
		default:
			break;
	}
}

void PrintDialog::print()
{
	QList<int> list;
	QList<QCheckBox *> box;
	int k = 0;

	box << ui->checkNum
		<< ui->checkName
		<< ui->checkUrl
		<< ui->checkCategories
		<< ui->checkLanguage
		<< ui->checkEpg;

	for(int i = 0; i < box.size(); i++) {
		if(box[i]->isChecked()) {
			list << k;
			k++;
		} else {
			list << -1;
		}
	}

	list << k;
	_print->channelList(_name, _playlist, list);
}
