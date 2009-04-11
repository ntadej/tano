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

#include "Effect.h"

#include "EffectManager.h"

#include "MediaObject.h"

namespace Phonon
{
namespace VLC_Backend
{

Effect::Effect(EffectManager * effectManager, int effectId, QObject * parent)
	: SinkNode(parent) {

	QList<EffectInfo *> effects = effectManager->getEffectList();
	if (effectId >= 0 && effectId < effects.size()) {
		_effectCommand = effects[effectId]->getCommand();
		_effectType = effects[effectId]->getType();
	} else {
		//Effect ID out of range
		Q_ASSERT(0);
	}
}

Effect::~Effect() {
}

void Effect::connectToMediaObject(PrivateMediaObject * mediaObject) {
	SinkNode::connectToMediaObject(mediaObject);

}

void Effect::disconnectFromMediaObject(PrivateMediaObject * mediaObject) {
	SinkNode::disconnectFromMediaObject(mediaObject);

}

QList<EffectParameter> Effect::parameters() const {
	QList<EffectParameter> params;
	return params;
}

QVariant Effect::parameterValue(const EffectParameter & param) const {
	QVariant def;
	return def;
}

void Effect::setParameterValue(const EffectParameter & param, const QVariant & newValue) {
}

}}	//Namespace Phonon::VLC_Backend
