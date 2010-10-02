/****************************************************************************
* PlaylistPage.cpp: First run wizard - Playlist selection page
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include "PlaylistPage.h"
#include "ui_PlaylistPage.h"

#include <QtCore/QDir>
#include <QtGui/QFileDialog>

#include "core/Settings.h"
#include "FirstRunWizard.h"

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
