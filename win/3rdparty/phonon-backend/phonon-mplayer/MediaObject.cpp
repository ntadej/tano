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

#include "MediaObject.h"

#include "SeekStack.h"

#include <libmplayer/MPlayerLoader.h>

//HACK for loadfile and volume commands
#include <libmplayer/MediaSettings.h>

#include <QtCore/QUrl>
#include <QtCore/QMetaType>
#include <QtCore/QTimer>

//Time in milliseconds before sending aboutToFinish() signal
//2 seconds
static const int ABOUT_TO_FINISH_TIME = 2000;

namespace Phonon
{
namespace MPlayer
{

MediaObject::MediaObject(QObject * parent)
	: MediaController(parent) {

	_videoWidgetId = 0;
	_prefinishMarkReachedEmitted = false;
	_aboutToFinishEmitted = false;

	//By default, no tick() signal
	//FIXME: Not implemented yet
	_tickInterval = 0;

	qRegisterMetaType<QMultiMap<QString, QString> >("QMultiMap<QString, QString>");

	//Run an empty MPlayerProcess in order to get the MPlayer version number
	//Using the MPlayer version number, we performs some tests (see MPlayerLoader.cpp)
	//and thus we need to know the MPlayer version number as soon as possible
	if (MPlayerProcess::getMPlayerVersion() == MPlayerProcess::MPLAYER_VERSION_NOTFOUND) {
		MPlayerLoader::startMPlayerVersion(this);
	}
	///

	connect(_process, SIGNAL(stateChanged(Phonon::State, Phonon::State)),
		SLOT(stateChangedInternal(Phonon::State, Phonon::State)));

	connect(_process, SIGNAL(endOfFileReached()),
		SLOT(endOfFileReached()));

	connect(_process, SIGNAL(tick(qint64)),
		SLOT(tickInternal(qint64)));

	connect(_process, SIGNAL(totalTimeChanged(qint64)),
		SIGNAL(totalTimeChanged(qint64)));

	connect(_process, SIGNAL(hasVideoChanged(bool)),
		SIGNAL(hasVideoChanged(bool)));

	connect(_process, SIGNAL(seekableChanged(bool)),
		SIGNAL(seekableChanged(bool)));

	connect(_process, SIGNAL(bufferStatus(int)),
		SIGNAL(bufferStatus(int)));

	connect(_process, SIGNAL(mediaLoaded()),
		SLOT(mediaLoaded()));

	connect(_process, SIGNAL(mediaDataChanged(const MediaData &)),
		SLOT(mediaDataChanged(const MediaData &)));
}

MediaObject::~MediaObject() {
}

MPlayerProcess * MediaObject::getMPlayerProcess() const {
	return _process;
}

void MediaObject::setVideoWidgetId(WId videoWidgetId) {
	_videoWidgetId = videoWidgetId;
}

void MediaObject::play() {
	qDebug() << __FUNCTION__;

	if (_process->currentState() == Phonon::PausedState) {
		//Pause is like resume inside MPlayer
		pause();
	} else {
		//Play the file
		_playRequestReached = true;

		//Clear subtitles/chapters...
		clearMediaController();

		MPlayerLoader::start(_process, _fileName, _videoWidgetId);
	}
}

void MediaObject::pause() {
	//Pause is like resume inside MPlayer
	_process->sendCommand("pause");
}

void MediaObject::stop() {
	_process->stop();
}

void MediaObject::seek(qint64 milliseconds) {
	static SeekStack * stack = new SeekStack(this);

	stack->pushSeek(milliseconds);
}

void MediaObject::seekInternal(qint64 milliseconds) {
	_process->sendCommand("seek " + QString::number(milliseconds / 1000.0) + " 2");
}

void MediaObject::tickInternal(qint64 currentTime) {
	qint64 totalTime = this->totalTime();

	//totalTime is < 0 means the media is a stream, thus no end
	if (totalTime > 0) {
		if (currentTime < totalTime - _prefinishMark) {
			_prefinishMarkReachedEmitted = false;
		} else {
			if (!_prefinishMarkReachedEmitted) {
				_prefinishMarkReachedEmitted = true;
				emit prefinishMarkReached(totalTime - currentTime);
			}
		}

		if (currentTime < totalTime - ABOUT_TO_FINISH_TIME) {
			_aboutToFinishEmitted = false;
		} else {
			if (!_aboutToFinishEmitted) {
				//Track is about to finish
				qDebug() << __FUNCTION__ << "aboutToFinish()";
				_aboutToFinishEmitted = true;
				emit aboutToFinish();
			}
		}
	}

	if (_tickInterval > 0) {
		//If _tickInternal == 0 means tick() signal is disabled
		//Default is _tickInternal = 0
		emit tick(currentTime);
	}
}

void MediaObject::loadMedia(const QString & fileName) {
	//Default MediaObject state should be Phonon::LoadingState
	if (_process->currentState() != Phonon::LoadingState) {
		qCritical() << __FUNCTION__ << "Current state is not Phonon::LoadingState:" << _process->currentState();
	}

	//Loads the media
	_playRequestReached = false;

	_fileName = fileName;

	qDebug() << __FUNCTION__ << _fileName;

	//Optimization:
	//wait to see if play() is run just after loadMedia()
	//100 milliseconds should be OK
	QTimer::singleShot(100, this, SLOT(loadMediaInternal()));
}

void MediaObject::loadMediaInternal() {
	if (_playRequestReached) {
		//We are already playing the media,
		//so there no need to load it
		return;
	}

	//See http://www.mplayerhq.hu/DOCS/tech/slave.txt
	//loadfile <file|url> <append>
	//Load the given file/URL, stopping playback of the current file/URL.
	//If <append> is nonzero playback continues and the file/URL is
	//appended to the current playlist instead.

	MPlayerLoader::loadMedia(_process, _fileName);
}

qint32 MediaObject::tickInterval() const {
	return _tickInterval;
}

void MediaObject::setTickInterval(qint32 tickInterval) {
	_tickInterval = tickInterval;
	/*if (_tickInterval <= 0) {
		_tickTimer->setInterval(50);
	} else {
		_tickTimer->setInterval(_tickInterval);
	}*/
}

bool MediaObject::hasVideo() const {
	return _process->hasVideo();
}

bool MediaObject::isSeekable() const {
	return _process->isSeekable();
}

qint64 MediaObject::currentTime() const {
	qint64 time = -1;
	Phonon::State state = _process->currentState();

	switch(state) {
	case Phonon::PausedState:
		time = _process->currentTime();
		break;
	case Phonon::BufferingState:
		time = _process->currentTime();
		break;
	case Phonon::PlayingState:
		time = _process->currentTime();
		break;
	case Phonon::StoppedState:
		time = 0;
		break;
	case Phonon::LoadingState:
		time = 0;
		break;
	case Phonon::ErrorState:
		time = -1;
		break;
	default:
		qCritical() << __FUNCTION__ << "Error: unknown Phonon::State:" << state;
	}

	return time;
}

Phonon::State MediaObject::state() const {
	return _process->currentState();
}

QString MediaObject::errorString() const {
	return _process->errorString();
}

Phonon::ErrorType MediaObject::errorType() const {
	return _process->errorType();
}

qint64 MediaObject::totalTime() const {
	return _process->totalTime();
}

MediaSource MediaObject::source() const {
	return _source;
}

QString MediaObject::sourceFileName(const MediaSource & source) {
	QString fileName;

	MediaSource::Type type = source.type();

	switch (type) {
	case MediaSource::Invalid:
		qCritical() << __FUNCTION__ << "Error: invalid/empty MediaSource";
		break;
	case MediaSource::LocalFile:
		fileName = source.fileName();
		break;
	case MediaSource::Url:
		fileName = source.url().toString();
		break;

	case MediaSource::Disc: {
		Phonon::DiscType discType = source.discType();
		MPlayerLoader::settings.opticalDeviceName = source.deviceName();
#ifdef NEW_TITLE_CHAPTER_HANDLING
		int title = currentTitle().index();
#else
		int title = currentTitle();
#endif	//NEW_TITLE_CHAPTER_HANDLING

		switch (discType) {
		case Phonon::NoDisc:
			qCritical() << __FUNCTION__ << "Error: the MediaSource::Disc doesn't specify which one (Phonon::NoDisc)";
			break;
		case Phonon::Cd:
			if (title == 0) {
				title = MPLAYER_DEFAULT_CDAUDIO_TITLE;
			}
			fileName = "cdda://" + QString::number(title);
			break;
		case Phonon::Dvd:
			if (title == 0) {
				title = MPLAYER_DEFAULT_DVD_TITLE;
			}
			//FIXME
			//fileName = "dvd://" + QString::number(title);
			fileName = "dvd://" + QString::number(MPLAYER_DEFAULT_DVD_TITLE);
			break;
		case Phonon::Vcd:
			if (title == 0) {
				title = MPLAYER_DEFAULT_VCD_TITLE;
			}
			fileName = "vcd://" + QString::number(title);
			break;
		default:
			qCritical() << __FUNCTION__ << "Error: unknown MediaSource::Disc:" << discType;
			break;
		}
		}
		break;

	case MediaSource::Stream:
		break;
	default:
		qCritical() << __FUNCTION__ << "Error: unknown MediaSource:" << type;
		break;
	}

	return fileName;
}

void MediaObject::setSource(const MediaSource & source) {
	_source = source;
	QString fileName(sourceFileName(_source));

	if (!fileName.isEmpty()) {
		qDebug() << __FUNCTION__ << "Source:" << fileName;

		loadMedia(fileName);

		emit currentSourceChanged(source);
	} else {
		//FIXME Possible to get into this case?
	}
}

void MediaObject::setNextSource(const MediaSource & source) {
	_nextSource = source;
	QString fileName(sourceFileName(_nextSource));

	if (!fileName.isEmpty()) {
		qDebug() << __FUNCTION__ << "Next source:" << fileName;

		QString quote("\"");
		if (_process->isRunning()) {
			//If MPlayerProcess is already running then use loadfile slave command
			_process->sendCommand("loadfile " + quote + fileName + quote + " " + QString::number(1));
		} else {
			//Otherwise back to create a new MPlayerProcess with the given media source
			setSource(source);
			play();
		}
	} else {
		//FIXME Possible to get into this case?
	}
}

qint32 MediaObject::prefinishMark() const {
	return _prefinishMark;
}

void MediaObject::setPrefinishMark(qint32 msecToEnd) {
	_prefinishMark = msecToEnd;
	if (currentTime() < totalTime() - _prefinishMark) {
		//Not about to finish
		_prefinishMarkReachedEmitted = false;
	}
}

qint32 MediaObject::transitionTime() const {
	return 0;
}

void MediaObject::setTransitionTime(qint32) {
}

void MediaObject::mediaDataChanged(const MediaData & mediaData) {
	QMultiMap<QString, QString> metaDataMap;
	metaDataMap.insert(QLatin1String("ARTIST"), mediaData.artist);
	metaDataMap.insert(QLatin1String("ALBUM"), mediaData.album);
	metaDataMap.insert(QLatin1String("TITLE"), mediaData.title);
	metaDataMap.insert(QLatin1String("DATE"), mediaData.date);
	metaDataMap.insert(QLatin1String("GENRE"), mediaData.genre);
	metaDataMap.insert(QLatin1String("TRACKNUMBER"), mediaData.track);
	metaDataMap.insert(QLatin1String("DESCRIPTION"), mediaData.comment);
	metaDataMap.insert(QLatin1String("COPYRIGHT"), mediaData.copyright);
	metaDataMap.insert(QLatin1String("ENCODEDBY"), mediaData.software);
	metaDataMap.insert(QLatin1String("LENGTH"), QString::number(mediaData.totalTime));

	metaDataMap.insert(QLatin1String("STREAM_URL"), mediaData.streamUrl);
	metaDataMap.insert(QLatin1String("STREAM_NAME"), mediaData.streamName);
	metaDataMap.insert(QLatin1String("STREAM_GENRE"), mediaData.streamGenre);
	metaDataMap.insert(QLatin1String("STREAM_WEBSITE"), mediaData.streamWebsite);

	//Other infos
	metaDataMap.insert(QLatin1String("DEMUXER"), mediaData.demuxer);
	if (mediaData.hasVideo) {
		metaDataMap.insert(QLatin1String("VIDEO_FORMAT"), mediaData.videoFormat);
		metaDataMap.insert(QLatin1String("VIDEO_BITRATE"), QString::number(mediaData.videoBitrate));
		metaDataMap.insert(QLatin1String("VIDEO_WIDTH"), QString::number(mediaData.videoWidth));
		metaDataMap.insert(QLatin1String("VIDEO_HEIGHT"), QString::number(mediaData.videoHeight));
		metaDataMap.insert(QLatin1String("VIDEO_FPS"), QString::number(mediaData.videoFPS));
		metaDataMap.insert(QLatin1String("VIDEO_ASPECT_RATIO"), QString::number(mediaData.videoAspectRatio));
		metaDataMap.insert(QLatin1String("AUDIO_FORMAT"), mediaData.audioFormat);
		metaDataMap.insert(QLatin1String("AUDIO_BITRATE"), QString::number(mediaData.audioBitrate));
		metaDataMap.insert(QLatin1String("AUDIO_RATE"), QString::number(mediaData.audioRate));
		metaDataMap.insert(QLatin1String("AUDIO_NCH"), QString::number(mediaData.audioNbChannels));
		metaDataMap.insert(QLatin1String("VIDEO_CODEC"), mediaData.videoCodec);
		metaDataMap.insert(QLatin1String("AUDIO_CODEC"), mediaData.audioCodec);
	} else {
		//Because of the mediaplayer example, see MediaPlayer::updateInfo()
		metaDataMap.insert(QLatin1String("BITRATE"), QString::number(mediaData.audioBitrate));
	}

	emit metaDataChanged(metaDataMap);
}

void MediaObject::mediaLoaded() {
	emit availableAudioChannelsChanged();
	emit availableSubtitlesChanged();

#ifdef NEW_TITLE_CHAPTER_HANDLING
	emit availableChaptersChanged();
	emit availableTitlesChanged();
#else
	emit availableChaptersChanged(availableChapters());
	emit availableTitlesChanged(availableTitles());
#endif	//NEW_TITLE_CHAPTER_HANDLING

	emit availableAnglesChanged(availableAngles());

	//Do we emit these signals?
	//MPlayer gives us only the current title
	//but no way to know the current angle and chapter :/
	//angleChanged(int angleNumber);
	//chapterChanged(int chapterNumber);
}

void MediaObject::stateChangedInternal(Phonon::State newState, Phonon::State oldState) {
	switch (newState) {
	case Phonon::LoadingState:
		qDebug() << __FUNCTION__ << "LoadingState";
		if (_nextSource.type() != MediaSource::Invalid) {
			//Means that we are playing the next MediaSource

			_source = _nextSource;

			//Make the next MediaSource invalid again
			_nextSource = MediaSource();
			emit currentSourceChanged(_source);
		}
		break;
	case Phonon::StoppedState:
		qDebug() << __FUNCTION__ << "StoppedState";
		break;
	case Phonon::PlayingState:
		qDebug() << __FUNCTION__ << "PlayingState";

		//HACK Bug inside MPlayer, the previous volume is not set again after the "loadfile" command
		//This should be removed when next version of MPlayer will be released
		if (MPlayerLoader::settings.volume >= 0) {
			_process->sendCommand("volume " + QString::number(MPlayerLoader::settings.volume) + " 1");
		}
		///

		break;
	case Phonon::BufferingState:
		qDebug() << __FUNCTION__ << "BufferingState";
		break;
	case Phonon::PausedState:
		qDebug() << __FUNCTION__ << "PausedState";
		break;
	case Phonon::ErrorState:
		qDebug() << __FUNCTION__ << "ErrorState";
		break;
	default:
		qCritical() << __FUNCTION__ << "Error: unknown state:" << newState;
		return;
	}

	if (newState == oldState) {
		qCritical() << __FUNCTION__ << "Error: 2 times the same state";
	}

	emit stateChanged(newState, oldState);
}

void MediaObject::endOfFileReached() {
	//Should be in state: Phonon::StoppedState;
	if (_process->currentState() != Phonon::StoppedState) {
		qCritical() << __FUNCTION__ << "Current state is not Phonon::StoppedState:" << _process->currentState();
	}

	//HACK: MPlayer cannot detect end of VBR MP3s!
	//If the MPlayer detects a length > to the real length
	//aboutToFinish() signal will never be emitted :/
	//Yes MPlayer devs have to fix this
	if (_process->previousState() == Phonon::PlayingState && !_aboutToFinishEmitted) {
		//Track is about to finish
		qDebug() << __FUNCTION__ << "aboutToFinish()";
		_aboutToFinishEmitted = true;
		emit aboutToFinish();
	}

	emit finished();
}

}}	//Namespace Phonon::MPlayer
