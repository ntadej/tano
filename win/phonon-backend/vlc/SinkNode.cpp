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

#include "SinkNode.h"

#include "MediaObject.h"

namespace Phonon
{
namespace VLC_Backend
{

SinkNode::SinkNode(QObject * parent)
	: QObject(parent) {

	_mediaObject = NULL;
}

SinkNode::~SinkNode() {
}

void SinkNode::connectToMediaObject(PrivateMediaObject * mediaObject) {
	if (_mediaObject) {
		qCritical() << __FUNCTION__ << "_mediaObject already connected";
	}

	_mediaObject = mediaObject;
}

void SinkNode::disconnectFromMediaObject(PrivateMediaObject * mediaObject) {
	if (_mediaObject != mediaObject) {
		qCritical() << __FUNCTION__ << "SinkNode was not connected to mediaObject";
	}
}

}}	//Namespace Phonon::VLC_Backend
