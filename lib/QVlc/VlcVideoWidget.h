/****************************************************************************
* QVlc - Qt and libVLC connector library
* VlcVideoWidget.h: Video widget
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

#ifndef QVLC_VLCVIDEOWIDGET_H_
#define QVLC_VLCVIDEOWIDGET_H_

#include <QtCore/QTimer>
#include <QtGui/QWidget>

class VlcVideoWidget : public QWidget
{
Q_OBJECT
public:
	VlcVideoWidget(QWidget *parent = 0);
	~VlcVideoWidget();

	WId getWinId() {return _widget->winId();};
	void setOsdSize(const int &width, const int &height);

protected:
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

signals:
	void full();
	void rightClick(const QPoint);
	void wheel(const bool);
	void mouseMove();
	void osdVisibility(const bool);

public slots:
	void disableMove();
	void enableMove();
	void controlFull();

	void teletext();
	int teletextPage();
	void setTeletextPage(const int &page);

	void setPreviousSettings();

	void setRatioOriginal();
	void setRatio1_1();
	void setRatio4_3();
	void setRatio16_9();
	void setRatio16_10();
	void setRatio2_21_1();
	void setRatio5_4();

	void setCropOriginal();
	void setCrop16_9();
	void setCrop16_10();
	void setCrop1_85_1();
	void setCrop2_21_1();
	void setCrop2_35_1();
	void setCrop2_39_1();
	void setCrop5_4();
	void setCrop5_3();
	void setCrop4_3();
	void setCrop1_1();

	void setFilterDisabled();
	void setFilterDiscard();
	void setFilterBlend();
	void setFilterMean();
	void setFilterBob();
	void setFilterLinear();
	void setFilterX();

private slots:
	void hideMouse();
	void applyPreviousSettings();

private:
	QWidget *_widget;
	QTimer *_timerMouse;
	QTimer *_timerSettings;

	bool _move;

	int _desktopWidth;
	int _desktopHeight;

	int _osdWidth;
	int _osdHeight;

	QString _currentRatio;
	QString _currentCrop;
	QString _currentFilter;
};

#endif // QVLC_VLCVIDEOWIDGET_H_
