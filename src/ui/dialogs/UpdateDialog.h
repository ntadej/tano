/****************************************************************************
* UpdateDialog.h: Dialog for checking for updates
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

#ifndef TANO_UPDATEDIALOG_H_
#define TANO_UPDATEDIALOG_H_

#include <QtGui/QAbstractButton>
#include <QtGui/QDialog>

#include "container/UpdateInfo.h"
#include "core/UpdateManager.h"

namespace Ui {
	class UpdateDialog;
}

class UpdateDialog : public QDialog
{
Q_OBJECT
public:
	UpdateDialog(QWidget *parent = 0);
	~UpdateDialog();

public slots:
	void check();
	void checkSilent();

protected:
	void changeEvent(QEvent *e);

signals:
	void newUpdate();

private slots:
	void action(QAbstractButton *button);
	void processUpdate(const QStringList &update,
					   const UpdateInfo &info);

private:
	Ui::UpdateDialog *ui;

	QString generateUrl(const QString &version);

	UpdateManager *_update;

	bool _silent;
};

#endif // TANO_UPDATEDIALOG_H_
