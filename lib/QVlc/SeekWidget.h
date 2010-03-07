/****************************************************************************
* QVlc - Qt and libVLC connector library
* SeekWidget.h: Seek widget
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

#ifndef QVLC_SEEKWIDGET_H_
#define QVLC_SEEKWIDGET_H_

#include <QtCore/QTimer>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

namespace QVlc
{
	class SeekWidget : public QWidget
	{
	Q_OBJECT
	public:
		SeekWidget(QWidget *parent = 0);
		~SeekWidget();

	private slots:
		void changeTime();
		void updateTime();

	private:
		QSlider *_seek;
		QLabel *_labelElapsed;
		QLabel *_labelFull;
		QTimer *_timer;
	};
};

#endif // QVLC_SEEKWIDGET_H_
