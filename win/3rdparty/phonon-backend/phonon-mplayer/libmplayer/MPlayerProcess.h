/*
 * MPlayer backend for the Phonon library
 * Copyright (C) 2006-2008  Ricardo Villalba <rvm@escomposlinux.org>
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

#ifndef MPLAYERPROCESS_H
#define MPLAYERPROCESS_H

#include <phonon/phononnamespace.h>

//For WId
#include <QtGui/QWidget>

#include <QtCore/QString>

#include "MyProcess.h"
#include "MediaData.h"

/**
 * Creates a new MPlayer process.
 *
 * Currently 1 MPlayerProcess => 1 real MPlayer process.
 * It might be good in the future to use MPlayer -idle option
 * in order to keep only 1 MPlayer process running.
 *
 * Permits to send commands to the MPlayer process via its slave mode
 * Permits to receive events from the MPlayer process
 *
 * Check the MPlayer slave mode documentation: http://www.mplayerhq.hu/DOCS/tech/slave.txt
 *
 * This code is from SMPlayer http://smplayer.sourceforge.net/
 * Ricardo Villalba did the hard work, I came later on to clean/refactor it.
 *
 * @see mplayer-input-cmdlist.txt
 * @see http://www.mplayerhq.hu/DOCS/tech/slave.txt
 * @author Ricardo Villalba
 * @author Tanguy Krotoff
 */
class MPlayerProcess : public MyProcess {
	Q_OBJECT
public:

	MPlayerProcess(QObject * parent);
	~MPlayerProcess();

	/**
	 * Starts the MPlayer process.
	 *
	 * URL to test:
	 * - Quicktime H.264: http://www.podtrac.com/pts/redirect.mov/zdpub.vo.llnwd.net/o2/crankygeeks/episode111/crankygeeks.111.mov
	 * - Video IPod: http://www.podtrac.com/pts/redirect.mp4/zdpub.vo.llnwd.net/o2/crankygeeks/episode111/crankygeeks.111.i.mp4
	 * - Windows Media Player: http://www.podtrac.com/pts/redirect.wmv/zdpub.vo.llnwd.net/o2/crankygeeks/episode111/crankygeeks.111.wmv
	 * - MPEG4: http://zdpub.vo.llnwd.net/o2/crankygeeks/episode111/crankygeeks.111.mp4
	 * - MP3 (audio only): http://www.podtrac.com/pts/redirect.mp3/zdpub.vo.llnwd.net/o2/crankygeeks/episode111/crankygeeks.111.mp3
	 *
	 * @param arguments options to give to the MPlayer process
	 * @param filename file/media/stream to play, can be an URL or dvd://
	 * @param videoWidgetId used with the -wid option
	 * @param seek position where to start inside the file/media/stream
	 */
	bool start(const QStringList & arguments, const QString & filename, WId videoWidgetId, qint64 seek);

	/**
	 * Stops the MPlayer process.
	 */
	void stop();

	/**
	 * Sends a command to the MPlayer process.
	 *
	 * Example: sendCommand("pause");
	 *
	 * @see http://www.mplayerhq.hu/DOCS/tech/slave.txt
	 */
	bool sendCommand(const QString & command);

	const MediaData & mediaData() const;

	bool hasVideo() const;
	bool isSeekable() const;

	qint64 currentTime() const;
	qint64 totalTime() const;

	bool isRunning() const;

	Phonon::State currentState() const;
	Phonon::State previousState() const;


	static const int MPLAYER_VERSION_NOTFOUND = -1;
	static const int MPLAYER_VERSION_FAILED = 0;
	static const int MPLAYER_1_0_RC1_SVN = 20372;
	static const int MPLAYER_1_0_RC2_SVN = 24722;

	/**
	 * Gets the MPlayer SVN revision number.
	 *
	 * @return SVN revision number; or 0 if fail to parse; or -1 if not read yet
	 */
	static int getMPlayerVersion();

	/**
	 * Last MPlayer error message.
	 *
	 * Call this method when in ErrorState.
	 *
	 * @return MPlayer error message
	 */
	QString errorString() const;

	/**
	 * Describes the severity when an error has occurred during playback.
	 *
	 * @see Phonon::ErrorType
	 */
	Phonon::ErrorType errorType() const;

signals:

	/**
	 * Emitted when the state of the media has changed.
	 *
	 * @param newState current state
	 * @param oldState previous state
	 * @see Phonon::State
	 */
	void stateChanged(Phonon::State newState, Phonon::State oldState);

	/**
	 * MPlayer reached the end of the stream/media/file.
	 */
	void endOfFileReached();

	/**
	 * Gives the current position in the stream in milliseconds.
	 *
	 * Example: "MPlayer is playing at 28,5 seconds from a video file of a 45,0 seconds length"
	 *
	 * @param time position in the stream in milliseconds (i.e 28,5 seconds)
	 * @see Phonon::MediaObject::tick()
	 */
	void tick(qint64 time);

	/**
	 * Gives the media/stream/file duration in milliseconds.
	 *
	 * @param totalTime media duration in milliseconds
	 */
	void totalTimeChanged(qint64 totalTime);

	/** A new frame has been received, gives the frame number (starts from 0). */
	void currentFrameNumberReceived(int number);

	/**
	 * If the stream contains a video or not.
	 *
	 * @see Phonon::MediaObject::hasVideoChanged()
	 */
	void hasVideoChanged(bool hasVideo);

	/**
	 * @param filename screenshot filename
	 */
	void screenshotSaved(const QString & filename);

	/**
	 * If the stream/media/file is seekable or not.
	 *
	 * @see Phonon::MediaObject::seekableChanged()
	 */
	void seekableChanged(bool isSeekable);

	/**
	 * Media has been loaded: everything is ready to do a 'play'.
	 */
	void mediaLoaded();

	/**
	 * Media/meta (artist, title, album, url...) data just changed.
	 */
	void mediaDataChanged(const MediaData & mediaData);

	/**
	 * New widget size computed by MPlayer.
	 *
	 * Should be applied to the widget that contains the MPlayer video.
	 */
	void videoWidgetSizeChanged(int width, int height);

	/**
	 * MPlayer tries to connect to a server.
	 *
	 * Only when the user tries to play an URL.
	 *
	 * Example: "Connecting to server www.podtrac.com[69.16.233.67]: 80..."
	 *
	 * @param message connecting message from MPlayer
	 */
	void connectingMessageReceived(const QString & message);

	/**
	 * MPlayer tries to resolve a domain name.
	 *
	 * Only when the user tries to play an URL.
	 *
	 * Example: "Resolving www.podtrac.com for AF_INET..."
	 *
	 * @param message resolving message from MPlayer
	 */
	void resolvingMessageReceived(const QString & message);

	/**
	 * A new audio stream/channel has been detected from the media/file/stream.
	 *
	 * Some files can contain several audio streams/channels.
	 * Example: DVD, Matroska
	 *
	 * @see http://en.wikipedia.org/wiki/Matroska
	 * @param id audio id to select if we choose this lang
	 * @param lang fr, en... / french, english...
	 */
	void audioChannelAdded(int id, const QString & lang);

	/**
	 * A new subtitle has been detected from the media/file/stream.
	 *
	 * Some files can contain several subtitles.
	 * Example: DVD, Matroska
	 *
	 * @see http://en.wikipedia.org/wiki/Matroska
	 * @param id subtitle id to select, start at number 0
	 * @param name can be the filename or the subtitle
	 *        language (fr, en... / french, english...) or anything else
	 * @param type subtitle type:
	 *        - file
	 *        - FIXME still to be done
	 */
	void subtitleAdded(int id, const QString & name, const QString & type);

	/**
	 * Current subtitle has changed.
	 *
	 * @param id the new current subtitle
	 */
	void subtitleChanged(int id);

	/**
	 * A new title has been detected from the media/file/stream.
	 *
	 * Title/chapter/angle DVD.
	 *
	 * @param id title id
	 * @param length title duration in milliseconds
	 */
	void titleAdded(int id, qint64 length);

	/**
	 * Current DVD/MKV title has changed.
	 *
	 * @param id the new current DVD/MKV title
	 */
	void titleChanged(int id);

	/**
	 * A new chapter has been detected from the media/file/stream.
	 *
	 * Title/chapter/angle DVD.
	 *
	 * @param titleId title id
	 * @param chapters number of chapters (or chapter id mkv)
	 */
	void chapterAdded(int titleId, int chapters);

	/**
	 * A new Matroska chapter has been detected from the media/file/stream.
	 *
	 * @param id chapter id
	 * @param title chapter' title
	 * @param from chapter beginning
	 * @param to chapter end
	 */
	void mkvChapterAdded(int id, const QString & title, const QString & from, const QString & to);

	/**
	 * A new angle has been detected from the media/file/stream.
	 *
	 * Title/chapter/angle DVD.
	 *
	 * @param titleId title id
	 * @param angles number of angles
	 */
	void angleAdded(int titleId, int angles);

	/**
	 * Percentage of MPlayer cache that has been filled.
	 *
	 * This is bufferization of a media over a network.
	 *
	 * @param percentFilled cache filled
	 */
	void bufferStatus(int percentFilled);



	void receivedCreatingIndex(QString);

private slots:

	/**
	 * Parses a line from the MPlayer process.
	 *
	 * @param line line to parse
	 */
	void parseLine(const QString & line);

	void finished(int exitCode, QProcess::ExitStatus exitStatus);

	void error(QProcess::ProcessError error);

private:

	/** Initializes/resets the private variables of this class. */
	void init();

	void changeState(Phonon::State newState);

	bool _endOfFileReached;

	MediaData _mediaData;

	/** MPlayer SVN revision number. */
	static int _mplayerVersion;

	/** Previous state. */
	Phonon::State _previousState;

	/** Current state. */
	Phonon::State _currentState;

	/** Last MPlayer error message. */
	QString _errorString;

	/** Describes the severity when an error has occurred during playback. */
	Phonon::ErrorType _errorType;

	/** Current DVD/MKV title id. */
	int _currentTitleId;
};

#endif	//MPLAYERPROCESS_H
