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

#ifndef PHONON_MPLAYER_MEDIAOBJECT_H
#define PHONON_MPLAYER_MEDIAOBJECT_H

#include "MediaController.h"

#include <libmplayer/MPlayerProcess.h>

#include <phonon/mediaobjectinterface.h>

#include <QtCore/QObject>

namespace Phonon
{
namespace MPlayer
{

/**
 *
 *
 * @author Tanguy Krotoff
 */
class MediaObject : public MediaController, public MediaObjectInterface {
	Q_OBJECT
	Q_INTERFACES(Phonon::MediaObjectInterface Phonon::AddonInterface)
public:

	MediaObject(QObject * parent);

	~MediaObject();

	/**
	 * Needed by VideoWidget
	 * Gets the MPlayerProcess used by this class.
	 *
	 * Cannot be NULL.
	 *
	 * @return the MPlayerProcess
	 */
	MPlayerProcess * getMPlayerProcess() const;

	/**
	 * Needed by VideoWidget
	 * Widget Id where MPlayer will show the videos.
	 */
	void setVideoWidgetId(WId videoWidgetId);

	//Needed by SeekStack
	void seekInternal(qint64 milliseconds);


	void play();
	void pause();
	void stop();

	void seek(qint64 milliseconds);

	qint32 tickInterval() const;
	void setTickInterval(qint32 tickInterval);

	bool hasVideo() const;
	bool isSeekable() const;

	qint64 currentTime() const;

	Phonon::State state() const;

	QString errorString() const;

	Phonon::ErrorType errorType() const;

	qint64 totalTime() const;

	MediaSource source() const;
	void setSource(const MediaSource & source);
	void setNextSource(const MediaSource & source);

	qint32 prefinishMark() const;
	void setPrefinishMark(qint32 msecToEnd);

	qint32 transitionTime() const;
	void setTransitionTime(qint32);

signals:

	void aboutToFinish();
	void bufferStatus(int percentFilled);
	void currentSourceChanged(const MediaSource & newSource);
	void finished();
	void hasVideoChanged(bool hasVideo);
	void metaDataChanged(const QMultiMap<QString, QString> & metaData);
	void prefinishMarkReached(qint32 msecToEnd);
	void seekableChanged(bool isSeekable);
	void stateChanged(Phonon::State newState, Phonon::State oldState);
	void tick(qint64 time);
	void totalTimeChanged(qint64 newTotalTime);

private slots:

	void loadMediaInternal();

	void mediaLoaded();

	void mediaDataChanged(const MediaData & mediaData);

	void stateChangedInternal(Phonon::State newState, Phonon::State oldState);

	void endOfFileReached();

	void tickInternal(qint64 currentTime);

private:

	void loadMedia(const QString & fileName);

	/** Internal code factorization. */
	QString sourceFileName(const MediaSource & source);

	/** Current MediaSource playing. */
	MediaSource _source;

	/** Next MediaSource to play. */
	MediaSource _nextSource;

	qint32 _prefinishMark;
	bool _prefinishMarkReachedEmitted;

	/** If the signal aboutToFinish() has been emitted or not. */
	bool _aboutToFinishEmitted;

	qint32 _tickInterval;

	bool _playRequestReached;

	QString _fileName;

	WId _videoWidgetId;
};

}}	//Namespace Phonon::MPlayer

#endif	//PHONON_MPLAYER_MEDIAOBJECT_H
