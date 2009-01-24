/*
 * MPlayer backend for the Phonon library
 * Copyright (C) 2006-2008  Ricardo Villalba <rvm@escomposlinux.org>
 * Copyright (C) 2007-2009  Tanguy Krotoff <tkrotoff@gmail.com>
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

#include "MPlayerVideoWidget.h"

#include <QtGui/QResizeEvent>
#include <QtCore/QDebug>

namespace Phonon
{
namespace MPlayer
{

MPlayerVideoWidget::MPlayerVideoWidget(QWidget * parent)
	: WidgetNoPaintEvent(parent) {

	_videoLayer = new WidgetNoPaintEvent(this);

	//Background color is black
	setBackgroundColor(Qt::black);

	//MPlayer color key for the DirectX backend
	//This is needed under Windows
	_videoLayer->setBackgroundColor(0x020202);
	///

	_aspectRatio = (double) 4 / 3;
	_scaleAndCrop = false;
}

MPlayerVideoWidget::~MPlayerVideoWidget() {
}

WId MPlayerVideoWidget::winId() const {
	return _videoLayer->winId();
}

void MPlayerVideoWidget::setAspectRatio(double aspectRatio) {
	_aspectRatio = aspectRatio;

	updateVideoWindow();
}

void MPlayerVideoWidget::setScaleAndCropMode(bool scaleAndCrop) {
	_scaleAndCrop = scaleAndCrop;

	updateVideoWindow();
}

void MPlayerVideoWidget::resizeEvent(QResizeEvent * event) {
	qDebug() << __FUNCTION__ << "Resize:" << event->size();

	updateVideoWindow();
}

void MPlayerVideoWidget::updateVideoWindow() const {
	int parentWidth = size().width();
	int parentHeight = size().height();

	int width, height;
	int x = 0;
	int y = 0;

	int pos1_h = (int) (parentWidth / _aspectRatio + 0.5);

	if (pos1_h <= parentHeight) {
		width = parentWidth;
		height = pos1_h;

		y = (parentHeight - height) / 2;
	} else {
		width = (int) (parentHeight * _aspectRatio + 0.5);
		height = parentHeight;

		x = (parentWidth - width) / 2;
	}

	if (_scaleAndCrop) {
		//Expand the video to the maximum size of the parent
		_videoLayer->move(0, 0);
		_videoLayer->resize(parentWidth, parentHeight);
	} else {
		//Respect the video aspect ratio
		_videoLayer->move(x, y);
		_videoLayer->resize(width, height);
	}
}

void MPlayerVideoWidget::setVideoSize(const QSize & videoSize) {
	_videoSize = videoSize;
	_aspectRatio = (double) _videoSize.width() / videoSize.height();

	updateVideoWindow();
}

QSize MPlayerVideoWidget::sizeHint() const {
	return _videoSize;
}

}}	//Namespace Phonon::MPlayer
