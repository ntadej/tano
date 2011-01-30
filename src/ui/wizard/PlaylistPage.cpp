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

#include <QtCore/QDir>
#include <QtGui/QFileDialog>

#include "core/Settings.h"
#include "FirstRunWizard.h"

#include "PlaylistPage.h"
#include "ui_PlaylistPage.h"

PlaylistPage::PlaylistPage(QWidget *parent)
	: QWizardPage(parent),
	ui(new Ui::PlaylistPage),
	_edit(new QLineEdit())
{
	ui->setupUi(this);

	Settings *settings = new Settings();
	ui->widgetSelectPlaylist->setPlaylist(settings->playlist());
	delete settings;

	registerField("playlist", _edit);
}

PlaylistPage::~PlaylistPage()
{
	delete ui;
	delete _edit;
}

void PlaylistPage::changeEvent(QEvent *e)
{
	QWizardPage::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

int PlaylistPage::nextId() const
{
	_edit->setText(ui->widgetSelectPlaylist->playlist());

	return FirstRunWizard::Conclusion;
}
