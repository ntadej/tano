/****************************************************************************
* QVlc - Qt and libVLC connector library
* VideoControl.h: Video and subtitles controller
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

#ifndef QVLC_VIDEOCONTROL_H_
#define QVLC_VIDEOCONTROL_H_

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>

namespace QVlc
{
	class VideoControl : public QObject
	{
	Q_OBJECT
	public:
		VideoControl(const QString &lang = "", QObject *parent = 0);
		~VideoControl();

		void mediaChange();
		void loadSubtitle(const QString &sub);

	private slots:
		void updateSubtitleActions();
		void updateSubtitles();
		void updateVideoActions();
		void updateVideo();

	signals:
		void actions(const QString, QList<QAction*>);

	private:
		QTimer *_timer;

		QList<QAction*> _actionSubList;
		QMap<QString,int> _mapSub;
		QActionGroup *_actionSubGroup;

		QList<QAction*> _actionVideoList;
		QMap<QString,int> _mapVideo;
		QActionGroup *_actionVideoGroup;

		bool _manualLanguage;
		QStringList _preferedLanguage;
	};
};

#endif // QVLC_VIDEOCONTROL_H_
