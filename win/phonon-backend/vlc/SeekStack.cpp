/*
 * VLC_Backend and MPlayer backends for the Phonon library
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

#include "SeekStack.h"

#include <QtCore/QTimer>
#include <QtCore/QDebug>

namespace Phonon
{
namespace VLC_Backend
{

SeekStack::SeekStack(MediaObject * mediaObject)
	: QObject(mediaObject) {

	_mediaObject = mediaObject;

	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), SLOT(popSeek()));
	_timer->setInterval(1000);
}

SeekStack::~SeekStack() {
}

void SeekStack::pushSeek(qint64 milliseconds) {
	qDebug() << __FUNCTION__ << "seek:" << milliseconds;

	disconnect(_mediaObject, SIGNAL(tickInternal(qint64)),
		_mediaObject, SLOT(tickInternalSlot(qint64)));

	_stack.push(milliseconds);

	if (!_timer->isActive()) {
		_timer->start();
		popSeek();
	}
}

void SeekStack::popSeek() {
	if (_stack.isEmpty()) {
		_timer->stop();
		reconnectTickSignal();
		return;
	}

	int milliseconds = _stack.pop();
	_stack.clear();

	qDebug() << __FUNCTION__ << "real seek:" << milliseconds;

	_mediaObject->seekInternal(milliseconds);

#ifdef PHONON_VLC
	reconnectTickSignal();
#endif	//PHONON_VLC
}

void SeekStack::reconnectTickSignal() {
	connect(_mediaObject, SIGNAL(tickInternal(qint64)),
		_mediaObject, SLOT(tickInternalSlot(qint64)));
}

}}	//Namespace Phonon::VLC_Backend
