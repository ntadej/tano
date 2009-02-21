/*
 * VLC and MPlayer backends for the Phonon library
 * Copyright (C) 2007-2008  Tanguy Krotoff <tkrotoff@gmail.com>
 * 					2009	Tadej Novak <tadej@pfusion.co.cc>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PHONON_VLC_MPLAYER_VIDEOWIDGET_H
#define PHONON_VLC_MPLAYER_VIDEOWIDGET_H

#include "SinkNode.h"

#include <phonon/videowidgetinterface.h>

#include "VLCVideoWidget.h"
typedef Phonon::VLC_MPlayer::VLCVideoWidget Widget;

namespace Phonon
{
namespace VLC_MPlayer
{

/**
 *
 *
 * @author Tanguy Krotoff
 */
class VideoWidget : public SinkNode, public VideoWidgetInterface {
	Q_OBJECT
	Q_INTERFACES(Phonon::VideoWidgetInterface)
public:

	VideoWidget(QWidget * parent);
	~VideoWidget();

	void connectToMediaObject(PrivateMediaObject * mediaObject);

	Phonon::VideoWidget::AspectRatio aspectRatio() const;
	void setAspectRatio(Phonon::VideoWidget::AspectRatio aspectRatio);

	qreal brightness() const;
	void setBrightness(qreal brightness);

	Phonon::VideoWidget::ScaleMode scaleMode() const;
	void setScaleMode(Phonon::VideoWidget::ScaleMode scaleMode);

	qreal contrast() const;
	void setContrast(qreal contrast);

	qreal hue() const;
	void setHue(qreal hue);

	qreal saturation() const;
	void setSaturation(qreal saturation);

	Widget * widget();

private slots:

	/**
	 * @see MPlayerProcess::videoWidgetSizeChanged()
	 */
	void videoWidgetSizeChanged(int width, int height);
	char* vlcAspectRatio() const;

private:

	Widget * _videoWidget;

	Phonon::VideoWidget::AspectRatio _aspectRatio;

	qreal _brightness;

	Phonon::VideoWidget::ScaleMode _scaleMode;

	qreal _contrast;

	qreal _hue;

	qreal _saturation;

	char* _vlcAspectRatio;
};

}}	//Namespace Phonon::VLC_MPlayer

#endif	//PHONON_VLC_MPLAYER_VIDEOWIDGET_H
