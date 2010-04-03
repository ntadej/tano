/****************************************************************************
* PlaylistPage.h: First run wizard - Playlist selection page
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

#ifndef TANO_PLAYLISTPAGE_H_
#define TANO_PLAYLISTPAGE_H_

#include <QtGui/QLineEdit>
#include <QtGui/QWizardPage>

namespace Ui
{
	class PlaylistPage;
}

class PlaylistPage : public QWizardPage
{
Q_OBJECT
public:
	PlaylistPage(QWidget *parent = 0);
	~PlaylistPage();

	int nextId() const;

protected:
	void changeEvent(QEvent *e);

private slots:
	void playlistBrowse();
	void playlistReset();

private:
	Ui::PlaylistPage *ui;

	QLineEdit *_edit;
};

#endif // TANO_PLAYLISTPAGE_H_
