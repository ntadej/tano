/*
 * MPlayer backend for the Phonon library
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

#include "VideoWidget.h"

#include "MediaObject.h"
#include "MPlayerVideoWidget.h"

#include <libmplayer/MPlayerProcess.h>

#include <QtGui/QWidget>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>

#include <QtCore/QtDebug>

namespace Phonon
{
namespace MPlayer
{

VideoWidget::VideoWidget(QWidget * parent)
	: SinkNode(parent) {

	_videoWidget = new MPlayerVideoWidget(parent);

	_aspectRatio = Phonon::VideoWidget::AspectRatioAuto;
	_scaleMode = Phonon::VideoWidget::FitInView;

	_brightness = 0;
	_contrast = 0;
	_hue = 0;
	_saturation = 0;
}

VideoWidget::~VideoWidget() {
}

void VideoWidget::connectToMediaObject(MediaObject * mediaObject) {
	SinkNode::connectToMediaObject(mediaObject);

	MPlayerProcess * process = _mediaObject->getMPlayerProcess();
	connect(process, SIGNAL(videoWidgetSizeChanged(int, int)),
		SLOT(videoWidgetSizeChanged(int, int)));

	_mediaObject->setVideoWidgetId(_videoWidget->winId());
}

Phonon::VideoWidget::AspectRatio VideoWidget::aspectRatio() const {
	return _aspectRatio;
}

void VideoWidget::setAspectRatio(Phonon::VideoWidget::AspectRatio aspectRatio) {
	qDebug() << __FUNCTION__ << "Aspect ratio:" << aspectRatio;

	_aspectRatio = aspectRatio;
	double ratio = (double) 4 / 3;

	switch (_aspectRatio) {

	//Let the decoder find the aspect ratio automatically from the media file (this is the default).
	case Phonon::VideoWidget::AspectRatioAuto:

	//Fits the video into the widget making the aspect ratio depend solely on the size of the widget.
	//This way the aspect ratio is freely resizeable by the user.
	case Phonon::VideoWidget::AspectRatioWidget:
		if (_mediaObject) {
			MPlayerProcess * process = _mediaObject->getMPlayerProcess();
			ratio = process->mediaData().videoAspectRatio;
		}
		break;

	case Phonon::VideoWidget::AspectRatio4_3:
		ratio = (double) 4 / 3;
		break;

	case Phonon::VideoWidget::AspectRatio16_9:
		ratio = (double) 16 / 9;
		break;

	default:
		qCritical() << __FUNCTION__ << "Error: unsupported AspectRatio:" << aspectRatio;
	}

	_videoWidget->setAspectRatio(ratio);
}

qreal VideoWidget::brightness() const {
	return _brightness;
}

void VideoWidget::setBrightness(qreal brightness) {
	_brightness = brightness;

	sendMPlayerCommand("brightness " + QString::number(_brightness * 100) + " 1");
}

Phonon::VideoWidget::ScaleMode VideoWidget::scaleMode() const {
	return _scaleMode;
}

void VideoWidget::setScaleMode(Phonon::VideoWidget::ScaleMode scaleMode) {
	//The ScaleMode enum describes how to treat aspect ratio during resizing of video

	_scaleMode = scaleMode;

	switch (_scaleMode) {

	//The video will be fitted to fill the view keeping aspect ratio
	case Phonon::VideoWidget::FitInView:
		_videoWidget->setScaleAndCropMode(false);
		break;

	//The video is scaled
	case Phonon::VideoWidget::ScaleAndCrop:
		_videoWidget->setScaleAndCropMode(true);
		break;

	default:
		qCritical() << __FUNCTION__ << "Error: unknown Phonon::VideoWidget::ScaleMode:" << _scaleMode;
	}
}

qreal VideoWidget::contrast() const {
	return _contrast;
}

void VideoWidget::setContrast(qreal contrast) {
	_contrast = contrast;

	sendMPlayerCommand("contrast " + QString::number(_contrast * 100) + " 1");
}

qreal VideoWidget::hue() const {
	return _hue;
}

void VideoWidget::setHue(qreal hue) {
	_hue = hue;

	sendMPlayerCommand("hue " + QString::number(_hue * 100) + " 1");
}

qreal VideoWidget::saturation() const {
	return _saturation;
}

void VideoWidget::setSaturation(qreal saturation) {
	_saturation = saturation;

	sendMPlayerCommand("saturation " + QString::number(_saturation * 100) + " 1");
}

QWidget * VideoWidget::widget() {
	return _videoWidget;
}

void VideoWidget::videoWidgetSizeChanged(int width, int height) {
	qDebug() << __FUNCTION__ << "Video width:" << width << "height:" << height;

	//I spent 2 full days for these few fucking lines of code!
	//It resizes dynamically the widget + the main window
	//I didn't find another way
	//Each line is very important!
	//If someone finds a better solution, please tell me!
	QWidget * parent = qobject_cast<QWidget *>(this->parent());

	QSize videoSize(width, height);
	videoSize.boundedTo(QApplication::desktop()->availableGeometry().size());

	_videoWidget->hide();
	_videoWidget->setVideoSize(videoSize);

#ifdef Q_OS_WIN
	QSize previousSize;
	if (parent) {
		previousSize = parent->minimumSize();
		parent->setMinimumSize(videoSize);
	}
#endif	//Q_OS_WIN

	_videoWidget->show();

#ifdef Q_OS_WIN
	if (parent) {
		parent->setMinimumSize(previousSize);
	}
#endif	//Q_OS_WIN
	///
}

}}	//Namespace Phonon::MPlayer
