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

#include "AudioOutput.h"

#include "MediaObject.h"

#include <libmplayer/MPlayerLoader.h>
#include <libmplayer/MediaSettings.h>

namespace Phonon
{
namespace MPlayer
{

AudioOutput::AudioOutput(QObject * parent)
	: SinkNode(parent) {

	_mediaObject = NULL;
}

AudioOutput::~AudioOutput() {
}

qreal AudioOutput::volume() const {
	if (MPlayerLoader::settings.volume == -1) {
		//-1 is the default value
		//MPlayer manpage: a value of -1 (the default) will not change the volume.
		return 1;
	} else {
		qreal vol = MPlayerLoader::settings.volume / 100.;
		return vol;
	}
}

void AudioOutput::setVolume(qreal volume) {
	//Saves the volume for the next running MPlayer process
	MPlayerLoader::settings.volume = volume * 100;

	//Sets the volume for the current running MPlayer process if any
	sendMPlayerCommand("volume " + QString::number(MPlayerLoader::settings.volume) + " 1");

	emit volumeChanged(volume);
}

int AudioOutput::outputDevice() const {
	return 0;
}

bool AudioOutput::setOutputDevice(int device) {
	return true;
}

bool AudioOutput::setOutputDevice(const Phonon::AudioOutputDevice & device) {
	return true;
}

}}	//Namespace Phonon::MPlayer
