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

#ifndef PHONON_VLC_MPLAYER_MEDIAOBJECT_H
#define PHONON_VLC_MPLAYER_MEDIAOBJECT_H

#include <phonon/mediaobjectinterface.h>

#include <QtCore/QObject>

namespace Phonon
{
namespace VLC_MPlayer
{

static const int MPLAYER_DEFAULT_DVD_TITLE = 1;

class SeekStack;

/**
 *
 *
 * @author Tanguy Krotoff
 */
class MediaObject : public QObject, public MediaObjectInterface {
	Q_OBJECT
	friend class SeekStack;
public:

	MediaObject(QObject * parent);
	virtual ~MediaObject();

	/**
	 * Widget Id where VLC or MPlayer will show the videos.
	 */
	void setVideoWidgetId(int videoWidgetId);

	void play();
	void seek(qint64 milliseconds);

	qint32 tickInterval() const;
	void setTickInterval(qint32 tickInterval);

	qint64 currentTime() const;
	Phonon::State state() const;
	Phonon::ErrorType errorType() const;
	MediaSource source() const;
	void setSource(const MediaSource & source);
	void setNextSource(const MediaSource & source);

	qint32 prefinishMark() const;
	void setPrefinishMark(qint32 msecToEnd);

	qint32 transitionTime() const;
	void setTransitionTime(qint32);

signals:

	void aboutToFinish();
	//void bufferStatus(int percentFilled);
	void currentSourceChanged(const MediaSource & newSource);
	void finished();
	void hasVideoChanged(bool hasVideo);
	void metaDataChanged(const QMultiMap<QString, QString> & metaData);
	void prefinishMarkReached(qint32 msecToEnd);
	void seekableChanged(bool isSeekable);
	void stateChanged(Phonon::State newState, Phonon::State oldState);
	void tick(qint64 time);
	void totalTimeChanged(qint64 newTotalTime);

	//Signal from MPlayerMediaObject and VLCMediaObject
	void stateChanged(Phonon::State newState);

	void tickInternal(qint64 time);

protected:

	virtual void loadMediaInternal(const QString & filename) = 0;
	virtual void playInternal() = 0;
	virtual void seekInternal(qint64 milliseconds) = 0;

	virtual qint64 currentTimeInternal() const = 0;

	int _videoWidgetId;

private slots:

	void stateChangedInternal(Phonon::State newState);

	void tickInternalSlot(qint64 time);

private:

	void loadMedia(const QString & filename);

	void resume();

	MediaSource _mediaSource;

	Phonon::State _currentState;

	qint32 _prefinishMark;
	bool _prefinishMarkReachedEmitted;

	bool _aboutToFinishEmitted;

	qint32 _tickInterval;
};

}}	//Namespace Phonon::VLC_MPlayer

#endif	//PHONON_VLC_MPLAYER_MEDIAOBJECT_H
