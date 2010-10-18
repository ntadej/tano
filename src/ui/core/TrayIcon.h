/****************************************************************************
* TrayIcon.h: Tray icon class
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

#ifndef TANO_TRAYICON_H_
#define TANO_TRAYICON_H_

#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QSystemTrayIcon>

class TrayIcon : public QSystemTrayIcon
{
Q_OBJECT
public:
	TrayIcon(QMenu *menu,
			 QObject *parent = 0);
	~TrayIcon();

public slots:
	void message(const QStringList &arg);
	void changeToolTip(const QString &text = "stop",
					   const QString &type = "main");

signals:
	void restoreClick();

private slots:
	void iconActivated(const QSystemTrayIcon::ActivationReason reason);

private:
	QString _currentlyPlaying;
	QString _currentlyRecording;
};

#endif // TANO_TRAYICON_H_
