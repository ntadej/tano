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

#ifndef PHONON_VLC_MPLAYER_EFFECTMANAGER_H
#define PHONON_VLC_MPLAYER_EFFECTMANAGER_H

#include <phonon/effectinterface.h>
#include <phonon/effectparameter.h>

#include <QtCore/QObject>

namespace Phonon
{
namespace VLC_MPlayer
{

class EffectInfo {
public:

	enum Type {
		AudioEffect,
		VideoEffect
	};

	EffectInfo(const QString & name, const QString & command, Type type) {
		_name = name;
		_command = command;
		_type = type;
	}

	QString getName() const {
		return _name;
	}

	QString getCommand() const {
		return _command;
	}

	Type getType() const {
		return _type;
	}

private:

	QString _name;

	QString _command;

	Type _type;
};

/**
 *
 *
 * @author Tanguy Krotoff
 */
class EffectManager : public QObject {
	Q_OBJECT
public:

	EffectManager(QObject * parent);
	~EffectManager();

	QList<EffectInfo *> getEffectList() const;

private:

	QList<EffectInfo *> _effectList;
};

}}	//Namespace Phonon::VLC_MPlayer

#endif	//PHONON_VLC_MPLAYER_EFFECTMANAGER_H
