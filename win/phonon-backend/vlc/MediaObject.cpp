/*
 * VLC and MPlayer backends for the Phonon library
 * Copyright (C) 2007-2008  Tanguy Krotoff <tkrotoff@gmail.com>
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

#include "MediaObject.h"

#include "SeekStack.h"

#include <QtCore/QUrl>
#include <QtCore/QMetaType>
#include <QtCore/QTimer>

//Time in milliseconds before sending aboutToFinish() signal
//2 seconds
static const int ABOUT_TO_FINISH_TIME = 2000;

namespace Phonon
{
namespace VLC_MPlayer
{

MediaObject::MediaObject(QObject * parent)
	: QObject(parent) {

	_currentState = Phonon::LoadingState;
	_videoWidgetId = 0;
	_prefinishMarkReachedEmitted = false;
	_aboutToFinishEmitted = false;

	//By default, no tick() signal
	//FIXME: Not implemented yet
	_tickInterval = 0;

	qRegisterMetaType<QMultiMap<QString, QString> >("QMultiMap<QString, QString>");

	connect(this, SIGNAL(stateChanged(Phonon::State)), SLOT(stateChangedInternal(Phonon::State)));

	connect(this, SIGNAL(tickInternal(qint64)), SLOT(tickInternalSlot(qint64)));
}

MediaObject::~MediaObject() {
}

void MediaObject::setVideoWidgetId(int videoWidgetId) {
	_videoWidgetId = videoWidgetId;
}

void MediaObject::play() {
	qDebug() << __FUNCTION__;

	if (_currentState == Phonon::PausedState) {
		resume();
	} else {
		//Play the file
		playInternal();
	}
}

void MediaObject::seek(qint64 milliseconds) {
	static SeekStack * stack = new SeekStack(this);

	stack->pushSeek(milliseconds);

	qint64 currentTime = this->currentTime();
	qint64 totalTime = this->totalTime();

	if (currentTime < totalTime - _prefinishMark) {
		_prefinishMarkReachedEmitted = false;
	}
	if (currentTime < totalTime - ABOUT_TO_FINISH_TIME) {
		_aboutToFinishEmitted = false;
	}
}

void MediaObject::tickInternalSlot(qint64 currentTime) {
	qint64 totalTime = this->totalTime();

	if (_tickInterval > 0) {
		//If _tickInternal == 0 means tick() signal is disabled
		//Default is _tickInternal = 0
		emit tick(currentTime);
	}

	if (_currentState == Phonon::PlayingState) {
		if (currentTime >= totalTime - _prefinishMark) {
			if (!_prefinishMarkReachedEmitted) {
				_prefinishMarkReachedEmitted = true;
				emit prefinishMarkReached(totalTime - currentTime);
			}
		}
		if (currentTime >= totalTime - ABOUT_TO_FINISH_TIME) {
			if (!_aboutToFinishEmitted) {
				//Track is about to finish
				_aboutToFinishEmitted = true;
				emit aboutToFinish();
			}
		}
	}
}

void MediaObject::loadMedia(const QString & filename) {
	//Default MediaObject state is Phonon::LoadingState
	_currentState = Phonon::LoadingState;

	//Loads the media
	loadMediaInternal(filename);
}

void MediaObject::resume() {
	pause();
}

qint32 MediaObject::tickInterval() const {
	return _tickInterval;
}

void MediaObject::setTickInterval(qint32 tickInterval) {
	_tickInterval = tickInterval;
	/*if (_tickInterval <= 0) {
		_tickTimer->setInterval(50);
	} else {
		_tickTimer->setInterval(_tickInterval);
	}*/
}

qint64 MediaObject::currentTime() const {
	qint64 time = -1;
	Phonon::State st = state();

	switch(st) {
	case Phonon::PausedState:
		time = currentTimeInternal();
		break;
	case Phonon::BufferingState:
		time = currentTimeInternal();
		break;
	case Phonon::PlayingState:
		time = currentTimeInternal();
		break;
	case Phonon::StoppedState:
		time = 0;
		break;
	case Phonon::LoadingState:
		time = 0;
		break;
	case Phonon::ErrorState:
		time = -1;
		break;
	default:
		qCritical() << __FUNCTION__ << "Error: unsupported Phonon::State:" << st;
	}

	return time;
}

Phonon::State MediaObject::state() const {
	return _currentState;
}

Phonon::ErrorType MediaObject::errorType() const {
	return Phonon::NormalError;
}

MediaSource MediaObject::source() const {
	return _mediaSource;
}

void MediaObject::setSource(const MediaSource & source) {
	qDebug() << __FUNCTION__;

	_mediaSource = source;

	switch (source.type()) {
	case MediaSource::Invalid:
		break;
	case MediaSource::LocalFile:
		loadMedia(_mediaSource.fileName());
		break;
	case MediaSource::Url:
		loadMedia(_mediaSource.url().toString());
		break;
	case MediaSource::Disc: {
		switch (source.discType()) {
		case Phonon::NoDisc:
			qCritical() << __FUNCTION__ << "Error: the MediaSource::Disc doesn't specify which one (Phonon::NoDisc)";
			return;
		case Phonon::Cd:
			loadMedia(_mediaSource.deviceName());
			break;
		case Phonon::Dvd:

#ifdef PHONON_VLC
			loadMedia("dvd://" + _mediaSource.deviceName());
#endif	//PHONON_VLC

#ifdef PHONON_MPLAYER
			loadMedia("dvd://" + QString::number(MPLAYER_DEFAULT_DVD_TITLE));
#endif	//PHONON_MPLAYER

			break;
		case Phonon::Vcd:
			loadMedia(_mediaSource.deviceName());
			break;
		default:
			qCritical() << __FUNCTION__ << "Error: unsupported MediaSource::Disc:" << source.discType();
			break;
		}
		}
		break;
	case MediaSource::Stream:
		break;
	default:
		qCritical() << __FUNCTION__ << "Error: unsupported MediaSource:" << source.type();
		break;
	}

	emit currentSourceChanged(source);
}

void MediaObject::setNextSource(const MediaSource & source) {
	setSource(source);
}

qint32 MediaObject::prefinishMark() const {
	return _prefinishMark;
}

void MediaObject::setPrefinishMark(qint32 msecToEnd) {
	_prefinishMark = msecToEnd;
	if (currentTime() < totalTime() - _prefinishMark) {
		//Not about to finish
		_prefinishMarkReachedEmitted = false;
	}
}

qint32 MediaObject::transitionTime() const {
	return 0;
}

void MediaObject::setTransitionTime(qint32) {
}

void MediaObject::stateChangedInternal(Phonon::State newState) {
	qDebug() << __FUNCTION__ << "newState:" << newState << "previousState:" << _currentState ;

	if (newState == _currentState) {
		//No state changed
		return;
	}

	//State changed
	Phonon::State previousState = _currentState;
	_currentState = newState;
	emit stateChanged(_currentState, previousState);
}

}}	//Namespace Phonon::VLC_MPlayer
