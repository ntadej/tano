/****************************************************************************
* PrintDialog.h: Dialog for printing settings
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

#ifndef TANO_PRINTDIALOG_H_
#define TANO_PRINTDIALOG_H_

#include <QtGui/QAbstractButton>
#include <QtGui/QDialog>

#include "core/Print.h"

namespace Ui {
	class PrintDialog;
}

class PrintDialog : public QDialog
{
Q_OBJECT
public:
	PrintDialog(const QString &name,
				PlaylistWidget *playlist,
				QWidget *parent = 0);
	~PrintDialog();

protected:
	void changeEvent(QEvent *e);

private slots:
	void action(QAbstractButton *button);

private:
	void print();

	Ui::PrintDialog *ui;

	Print *_print;

	QString _name;
	PlaylistWidget *_playlist;
};

#endif // TANO_PRINTDIALOG_H_
