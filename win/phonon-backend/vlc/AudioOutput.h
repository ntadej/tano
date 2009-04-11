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

#ifndef PHONON_VLC_MPLAYER_AUDIOOUTPUT_H
#define PHONON_VLC_MPLAYER_AUDIOOUTPUT_H

#include "SinkNode.h"

#include <phonon/audiooutputinterface.h>

namespace Phonon
{
namespace VLC_Backend
{
class AudioOutput : public SinkNode, public AudioOutputInterface {
	Q_OBJECT
	Q_INTERFACES(Phonon::AudioOutputInterface)
public:

	AudioOutput(QObject * parent);
	~AudioOutput();

	qreal volume() const;
	void setVolume(qreal volume);

	int outputDevice() const;
	bool setOutputDevice(int);
	bool setOutputDevice(const Phonon::AudioOutputDevice & device);

signals:

	void volumeChanged(qreal volume);

private:

};

}}	//Namespace Phonon::VLC_Backend

#endif	//PHONON_VLC_AUDIOOUTPUT_H
