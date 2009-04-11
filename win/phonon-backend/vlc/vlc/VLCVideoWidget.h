/*
 * VLC_Backend backend for the Phonon library
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

#ifndef PHONON_VLCVIDEOWIDGET_H
#define PHONON_VLCVIDEOWIDGET_H

#include "../WidgetNoPaintEvent.h"

#include <QtGui/QWidget>

class QResizeEvent;

namespace Phonon
{
namespace VLC_Backend
{

/**
 * Widget where to show VLC_Backend video.
 *
 * @author Tanguy Krotoff
 */
class VLCVideoWidget : public WidgetNoPaintEvent {
	Q_OBJECT
public:

	VLCVideoWidget(QWidget * parent);
	~VLCVideoWidget();

	void setVideoSize(const QSize & videoSize);
	void setAspectRatio(double aspectRatio);
	void setScaleAndCropMode(bool scaleAndCrop);

	QSize sizeHint() const;

private:

	void resizeEvent(QResizeEvent * event);

	/** Original size of the video, needed for sizeHint(). */
	QSize _videoSize;
};

}}	//Namespace Phonon::VLC_Backend

#endif	//PHONON_VLCVIDEOWIDGET_H
