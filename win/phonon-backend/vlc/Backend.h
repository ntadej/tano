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

#ifndef PHONON_VLC_MPLAYER_BACKEND_H
#define PHONON_VLC_MPLAYER_BACKEND_H

#include <phonon/objectdescription.h>
#include <phonon/backendinterface.h>

#include <QtCore/QList>
#include <QtCore/QPointer>
#include <QtCore/QStringList>
#include <QtCore/QObject>

namespace Phonon
{
namespace VLC_MPlayer
{

class EffectManager;

/**
 * VLC backend class.
 *
 * @author Tanguy Krotoff
 */
class Backend : public QObject, public BackendInterface {
	Q_OBJECT
	Q_INTERFACES(Phonon::BackendInterface)
public:

	Backend(QObject * parent = NULL, const QVariantList & args = QVariantList());
	~Backend();

	QObject * createObject(BackendInterface::Class, QObject * parent, const QList<QVariant> & args);

	bool supportsVideo() const;
	bool supportsOSD() const;
	bool supportsFourcc(quint32 fourcc) const;
	bool supportsSubtitles() const;

	void freeSoundcardDevices();

	QList<int> objectDescriptionIndexes(ObjectDescriptionType type) const;
	QHash<QByteArray, QVariant> objectDescriptionProperties(ObjectDescriptionType type, int index) const;

	bool startConnectionChange(QSet<QObject *> nodes);
	bool connectNodes(QObject * source, QObject * sink);
	bool disconnectNodes(QObject * source, QObject * sink);
	bool endConnectionChange(QSet<QObject *> nodes);

public slots:

	/**
	 * @see http://en.wikipedia.org/wiki/Mime_type
	 */
	QStringList availableMimeTypes() const;

	/**
	 * Test of introspection.
	 */
	QString toString() const;

signals:

	void objectDescriptionChanged(ObjectDescriptionType);

private:

	mutable QStringList _supportedMimeTypes;

	EffectManager * _effectManager;
};

}}	//Namespace Phonon::VLC_MPlayer

#endif	//PHONON_VLC_MPLAYER_BACKEND_H
