/****************************************************************************
* QVlc - Qt and libVLC connector library
* VlcControl.h: Subtitles and audio channels controller
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

#ifndef QVLC_VLCCONTROL_H_
#define QVLC_VLCCONTROL_H_

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>

class VlcControl : public QObject
{
Q_OBJECT
public:
	VlcControl(const QString &lang = "");
	~VlcControl();

	void update();

private slots:
	void updateActionsAudio();
	void updateActionsVideo();
	void updateAudio();
	void updateSub();

	void checkPlayingState();

signals:
	void vlcAction(const QString, QList<QAction*>);
	void stateChanged(const int);

private:
	QTimer *_timer;
	QTimer *_check;

	QList<QAction*> _audioList;
	QMap<QString,int> _audioMap;
	QActionGroup *_audioGroup;

	QList<QAction*> _subList;
	QMap<QString,int> _subMap;
	QActionGroup *_subGroup;

	bool _manualLanguage;
	QStringList _preferedLanguage;
};

#endif // QVLC_VLCCONTROL_H_
