/*
 * MPlayer backend for the Phonon library
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

#include "EffectManager.h"

namespace Phonon
{
namespace MPlayer
{

EffectInfo::EffectInfo(const QString & name, const QString & command, Type type) {
	_name = name;
	_command = command;
	_type = type;
}

EffectInfo::~EffectInfo() {
}

QString EffectInfo::name() const {
	return _name;
}

QString EffectInfo::command() const {
	return _command;
}

EffectInfo::Type EffectInfo::type() const {
	return _type;
}


EffectManager::EffectManager(QObject * parent)
	: QObject(parent) {

	//Audio effects
	_effectList.append(new EffectInfo("(Audio) Karaoke", "karaoke", EffectInfo::AudioEffect));
	_effectList.append(new EffectInfo("(Audio) Extra stereo", "extrastereo", EffectInfo::AudioEffect));
	_effectList.append(new EffectInfo("(Audio) Volume normalization", "volnorm=2", EffectInfo::AudioEffect));

	//Deinterlace
	_effectList.append(new EffectInfo("(Video) Deinterlace: Lowpass5", "pp=l5", EffectInfo::VideoEffect));
	_effectList.append(new EffectInfo("(Video) Deinterlace: Yadif (normal)", "yadif", EffectInfo::VideoEffect));
	_effectList.append(new EffectInfo("(Video) Deinterlace: Yadif (double framerate)", "yadif=1", EffectInfo::VideoEffect));
	_effectList.append(new EffectInfo("(Video) Deinterlace: Linear Blend", "pp=lb", EffectInfo::VideoEffect));
	_effectList.append(new EffectInfo("(Video) Deinterlace: Kerndeint", "kerndeint=5", EffectInfo::VideoEffect));

	//Rotate
	_effectList.append(new EffectInfo("(Video) Rotate: clockwise flip", "rotate=0", EffectInfo::VideoEffect));
	_effectList.append(new EffectInfo("(Video) Rotate: clockwise", "rotate=1", EffectInfo::VideoEffect));
	_effectList.append(new EffectInfo("(Video) Rotate: counter clockwise", "rotate=2", EffectInfo::VideoEffect));
	_effectList.append(new EffectInfo("(Video) Rotate: counter clockwise flip", "rotate=3", EffectInfo::VideoEffect));

	//Denoise
	_effectList.append(new EffectInfo("(Video) Denoise: soft", "hqdn3d=2:1:2", EffectInfo::VideoEffect));
	_effectList.append(new EffectInfo("(Video) Denoise: normal", "hqdn3d", EffectInfo::VideoEffect));

	//Deblock
	_effectList.append(new EffectInfo("(Video) Deblock", "pp=vb/hb", EffectInfo::VideoEffect));

	//Dering
	_effectList.append(new EffectInfo("(Video) Dering", "pp=dr", EffectInfo::VideoEffect));

	//Addnoise
	_effectList.append(new EffectInfo("(Video) Addnoise", "noise=9ah:5ah", EffectInfo::VideoEffect));

	//Postprocessing
	//FIXME does not work
	//_effectList.append(new EffectInfo("(Video) Postprocessing", "pp -autoq 6", EffectInfo::VideoEffect));
}

EffectManager::~EffectManager() {
	_effectList.clear();
}

QList<EffectInfo *> EffectManager::effectList() const {
	return _effectList;
}

}}	//Namespace Phonon::MPlayer
