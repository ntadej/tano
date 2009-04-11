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

#ifndef PHONON_VLCMEDIAOBJECT_H
#define PHONON_VLCMEDIAOBJECT_H

#include "VLCMediaController.h"

#include "../MediaObject.h"

#include <phonon/mediaobjectinterface.h>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMultiMap>

namespace Phonon
{
namespace VLC_Backend
{

/**
 * VLC_Backend MediaObject.
 *
 * This is the "brain" of the VLC_Backend backend.
 * VLCMediaObject uses libvlc in order to send commands and receive
 * events from the VLC_Backend.
 *
 * Encapsulates VLC_Backend specific code.
 * Take care of libvlc events via libvlc_callback()
 *
 * @see MediaObject
 * @author Tanguy Krotoff
 */
class VLCMediaObject : public MediaObject, public VLCMediaController {
	Q_OBJECT
	Q_INTERFACES(Phonon::MediaObjectInterface Phonon::AddonInterface)
public:

	VLCMediaObject(QObject * parent);
	~VLCMediaObject();

	void pause();
	void stop();

	bool hasVideo() const;
	bool isSeekable() const;

	qint64 totalTime() const;

	QString errorString() const;

signals:

	//MediaController signals
	void availableSubtitlesChanged();
	void availableAudioChannelsChanged();

	void availableChaptersChanged();
	void availableTitlesChanged();

	void availableChaptersChanged(int availableChapters);
	void availableTitlesChanged(int availableTitles);

	void availableAnglesChanged(int availableAngles);
	void angleChanged(int angleNumber);
	void chapterChanged(int chapterNumber);
	void titleChanged(int titleNumber);

	/**
	 * New widget size computed by VLC_Backend.
	 *
	 * Should be applied to the widget that contains the VLC_Backend video.
	 */
	void videoWidgetSizeChanged(int width, int height);

protected:

	void loadMediaInternal(const QString & filename);
	void playInternal();
	void seekInternal(qint64 milliseconds);

	qint64 currentTimeInternal() const;

private slots:

	void loadMediaInternal();

private:

	/**
	 * Connects libvlc_callback() to all vlc events.
	 *
	 * @see libvlc_callback()
	 */
	void connectToAllVLCEvents();

	/**
	 * Retrieves (i.e ARTIST, TITLE, ALBUM...) meta data of a file.
	 */
	void updateMetaData();

	/**
	 * Libvlc callback.
	 *
	 * Receives all vlc events.
	 *
	 * Warning: owns by libvlc thread.
	 *
	 * @see connectToAllVLCEvents()
	 * @see libvlc_event_attach()
	 */
	static void libvlc_callback(const libvlc_event_t * event, void * user_data);

	void unloadMedia();

	void setVLCWidgetId();

	//MediaPlayer
	//libvlc_media_player_t * _vlcMediaPlayer;
	libvlc_event_manager_t * _vlcMediaPlayerEventManager;

	//Media
	libvlc_media_t * _vlcMedia;
	libvlc_event_manager_t * _vlcMediaEventManager;

	//MediaDiscoverer
	libvlc_media_discoverer_t * _vlcMediaDiscoverer;
	libvlc_event_manager_t * _vlcMediaDiscovererEventManager;

	bool _playRequestReached;

	qint64 _totalTime;

	bool _hasVideo;

	bool _seekable;
};

}}	//Namespace Phonon::VLC_Backend

#endif	//PHONON_VLCMEDIAOBJECT_H
