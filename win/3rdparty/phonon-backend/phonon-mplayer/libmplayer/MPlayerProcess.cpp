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

#include "MPlayerProcess.h"

#include "MPlayerVersion.h"

#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QtDebug>

#ifdef Q_OS_WIN
	//Under Windows, mplayer.exe should be inside a subdirectory named mplayer
	static const char * MPLAYER_EXE = "mplayer/mplayer.exe";
#else
	static const char * MPLAYER_EXE = "mplayer";
#endif

/** MPlayer works using seconds, we prefer to work using milliseconds. */
static const double SECONDS_CONVERTION = 1000.0;

/** MPlayer SVN revision number not found yet. */
int MPlayerProcess::_mplayerVersion = MPlayerProcess::MPLAYER_VERSION_NOTFOUND;

MPlayerProcess::MPlayerProcess(QObject * parent)
	: MyProcess(parent) {

	init();

	connect(this, SIGNAL(lineAvailable(const QString &)),
		SLOT(parseLine(const QString &)));

	connect(this, SIGNAL(finished(int, QProcess::ExitStatus)),
		SLOT(finished(int, QProcess::ExitStatus)));

	connect(this, SIGNAL(error(QProcess::ProcessError)),
		SLOT(error(QProcess::ProcessError)));
}

MPlayerProcess::~MPlayerProcess() {
}

void MPlayerProcess::init() {
	_endOfFileReached = false;
	_mediaData.clear();
	_previousState = Phonon::LoadingState;
	_currentState = Phonon::LoadingState;
	_errorString.clear();
	_errorType = Phonon::NoError;
	_currentTitleId = 0;
}

bool MPlayerProcess::start(const QStringList & arguments, const QString & filename, WId videoWidgetId, qint64 seek) {
	//Stop MPlayerProcess if it is already running
	if (isRunning()) {
		stop();
	}

	init();

	QStringList args;
	args << arguments;

	if (videoWidgetId > 0) {
		//Attach MPlayer video output to our widget
		_mediaData.videoWidgetId = videoWidgetId;
		args << "-wid";
		args << QString::number((qint64) _mediaData.videoWidgetId);
	}

	//If seek < 5 it's better to allow the video to start from the beginning
	if (seek > 5) {
		_mediaData.currentTime = seek;
		args << "-ss";
		args << QString::number(_mediaData.currentTime / SECONDS_CONVERTION);
	}

	//File to play
	_mediaData.filename = filename;
	args << filename;

	MyProcess::start(MPLAYER_EXE, args);
	return waitForStarted();
}

void MPlayerProcess::stop() {
	if (!isRunning()) {
		qWarning() << __FUNCTION__ << "MPlayer not running";
		return;
	}

	sendCommand("quit");

	qDebug() << __FUNCTION__ << "Finishing MPlayer...";
	if (!waitForFinished(5000)) {
		kill();
	}

	qDebug() << __FUNCTION__ << "MPlayer finished";
}

bool MPlayerProcess::sendCommand(const QString & command) {
	bool result = false;

	qDebug() << __FUNCTION__ << "Command:" << command;

	if (!command.isEmpty()) {
		if (isRunning()) {
			qint64 nbBytes = write(command.toLocal8Bit() + "\n");
			if (nbBytes != -1) {
				result = true;
			} else {
				qWarning() << __FUNCTION__ << "Error: couldn't write inside MPlayer process";
			}
		} else {
			qWarning() << __FUNCTION__ << "Error: MPlayer process not running";
		}
	} else {
		qWarning() << __FUNCTION__ << "Error: empty MPlayer command";
	}

	return result;
}

const MediaData & MPlayerProcess::mediaData() const {
	return _mediaData;
}

bool MPlayerProcess::hasVideo() const {
	return _mediaData.hasVideo;
}

bool MPlayerProcess::isSeekable() const {
	return _mediaData.isSeekable;
}

qint64 MPlayerProcess::currentTime() const {
	return _mediaData.currentTime;
}

qint64 MPlayerProcess::totalTime() const {
	return _mediaData.totalTime;
}

bool MPlayerProcess::isRunning() const {
	//Use QProcess state
	return state() == QProcess::Running;
}

int MPlayerProcess::getMPlayerVersion() {
	return _mplayerVersion;
}

QString MPlayerProcess::errorString() const {
	return _errorString;
}

Phonon::ErrorType MPlayerProcess::errorType() const {
	return _errorType;
}

//General
static QRegExp rx_av("^[AV]: *([0-9,:.-]+)");
static QRegExp rx_frame("^[AV]:.* (\\d+)\\/.\\d+");
static QRegExp rx_generic("^(.*)=(.*)");
static QRegExp rx_audio_mat("^ID_AID_(\\d+)_(LANG|NAME)=(.*)");
static QRegExp rx_winresolution("^VO: \\[(.*)\\] (\\d+)x(\\d+) => (\\d+)x(\\d+)");
static QRegExp rx_ao("^AO: \\[(.*)\\]");
static QRegExp rx_paused("^ID_PAUSED");
static QRegExp rx_novideo("^Video: no video");
static QRegExp rx_play("^Starting playback...");
static QRegExp rx_playing("^Playing");	//"Playing" does not mean the file is actually playing but only loading
static QRegExp rx_file_not_found("^File not found:");
static QRegExp rx_endoffile("^Exiting... \\(End of file\\)");

//Streaming
static QRegExp rx_connecting("^Connecting to server (.*)...");
static QRegExp rx_resolving("^Resolving (.*)...");
static QRegExp rx_resolving_failed("^Couldn't resolve name for ");
static QRegExp rx_cache_fill("^Cache fill: (.*)%");
static QRegExp rx_read_failed("^Read failed.");	//"Read failed" for a streaming media

//Screenshot
static QRegExp rx_screenshot("^\\*\\*\\* screenshot '(.*)'");

//DVD/Mkv titles/chapters/angles
static QRegExp rx_titles("^ID_DVD_TITLES=(\\d+)");
static QRegExp rx_title("^ID_DVD_TITLE_(\\d+)_(LENGTH|CHAPTERS|ANGLES)=(.*)");
//[mkv] Chapter 1 from 00:15:02.080 to 00:00:00.000, Plus l'on Approche de César
static QRegExp rx_mkvchapters("\\[mkv\\] Chapter (\\d+) from (.*) to (.*), (.*)");
static QRegExp rx_create_index("^Generating Index:.*");

//VCD
static QRegExp rx_vcd("^ID_VCD_TRACK_(\\d+)_MSF=(.*)");

//Audio CD
static QRegExp rx_cdda("^ID_CDDA_TRACK_(\\d+)_MSF=(.*)");

//Subtitles
static QRegExp rx_subtitle("^ID_(SUBTITLE|FILE_SUB|VOBSUB)_ID=(\\d+)");
static QRegExp rx_sid("^ID_(SID|VSID)_(\\d+)_(LANG|NAME)=(.*)");
//static QRegExp rx_subtitle_file("^ID_FILE_SUB_FILENAME=(.*)");
static QRegExp rx_subtitle_file("^SUB: Added subtitle file \\((\\d+)\\): (.*)");

//Meta data infos
static QRegExp rx_clip_title("^(name|title): (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_artist("^artist: (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_author("^author: (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_album("^album: (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_genre("^genre: (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_date("^(creation date|year): (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_track("^track: (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_copyright("^copyright: (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_comment("^comment: (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_software("^software: (.*)", Qt::CaseInsensitive);

//Radio streaming infos
static QRegExp rx_stream_title("^.* StreamTitle='(.*)';StreamUrl='(.*)';");
static QRegExp rx_stream_title_only("^.* StreamTitle='(.*)';");
static QRegExp rx_stream_name("^Name   : (.*)");
static QRegExp rx_stream_genre("^Genre  : (.*)");
static QRegExp rx_stream_website("^Website: (.*)");

void MPlayerProcess::parseLine(const QString & tmp) {
	QString line = tmp;

	//Skip empty lines
	if (line.isEmpty()) {
		return;
	}

	//Skip repeatitive lines
	if (line.contains("Too many buffered pts") || line.contains("pts value <= previous")) {
		return;
	}

	//Parse A: V: line
	if (rx_av.indexIn(line) > -1) {
		_mediaData.currentTime = (qint64) (rx_av.cap(1).toDouble() * SECONDS_CONVERTION);

		if (_currentState != Phonon::PlayingState) {
			qDebug() << __FUNCTION__ << "Starting time:" << _mediaData.currentTime;
			changeState(Phonon::PlayingState);

			//OK, now all the media datas should be in clean state
			emit mediaLoaded();
		}

		emit tick(_mediaData.currentTime);

		//Check for frame number
		if (rx_frame.indexIn(line) > -1) {
			int frame = rx_frame.cap(1).toInt();
			//qDebug() << __FUNCTION__ << "Frame number:" << frame;
			emit currentFrameNumberReceived(frame);
		}
	}

	//Parse other things
	else {
		qDebug() << "MPlayer:" << line;

		//Loading the file/stream/media
		//Becarefull! "Playing" does not mean the file is playing but only loading
		if (rx_playing.indexIn(line) > -1) {
			changeState(Phonon::LoadingState);
		}

		//File not found
		else if (rx_file_not_found.indexIn(line) > -1) {
			_errorString = "File not found";
			_errorType = Phonon::NormalError;
			changeState(Phonon::ErrorState);
		}

		//Screenshot
		else if (rx_screenshot.indexIn(line) > -1) {
			const QString filename = rx_screenshot.cap(1);
			qDebug() << __FUNCTION__ << "Screenshot:" << filename;
			emit screenshotSaved(filename);
		}

		//End of file
		else if (rx_endoffile.indexIn(line) > -1) {
			qDebug() << __FUNCTION__ << "End of file detected";

			//In case of playing VCDs or DVDs, maybe the first title
			//is not playable, so the GUI doesn't get the info about
			//available titles. So if we received the end of file
			//first let's pretend the file has started so the GUI can have
			//the data.
			if (_currentState != Phonon::PlayingState) {
				changeState(Phonon::PlayingState);
			}

			//Sends the endOfFileReached() signal once the process is finished, not now!
			_endOfFileReached = true;
		}

		//Window resolution
		else if (rx_winresolution.indexIn(line) > -1) {
			int width = rx_winresolution.cap(4).toInt();
			int height = rx_winresolution.cap(5).toInt();

			qDebug() << __FUNCTION__ << "Video driver:" << rx_winresolution.cap(1);

			//Now we know the real video size
			emit videoWidgetSizeChanged(width, height);

			//Ok, now we can be in PlayingState if we have a video stream
			//If we have only an audio stream, we are already in PlayingState
			changeState(Phonon::PlayingState);

			//emit mplayerFullyLoaded();
		}

		//No video
		else if (rx_novideo.indexIn(line) > -1) {
			_mediaData.hasVideo = false;
			qDebug() << __FUNCTION__ << "Video:" << _mediaData.hasVideo;
			emit hasVideoChanged(_mediaData.hasVideo);
			//emit mplayerFullyLoaded();
		}

		//Pause
		else if (rx_paused.indexIn(line) > -1) {
			changeState(Phonon::PausedState);
		}

		//Stream title and url
		else if (rx_stream_title.indexIn(line) > -1) {
			QString title = rx_stream_title.cap(1);
			QString url = rx_stream_title.cap(2);
			qDebug() << __FUNCTION__ << "Stream title:" << title;
			qDebug() << __FUNCTION__ << "Stream url:" << url;
			_mediaData.title = title;
			if (!url.isEmpty() && url.at(url.size() - 1) == '/') {
				url.remove(url.size() - 1, 1);
			}
			_mediaData.streamUrl = url;

			emit mediaDataChanged(_mediaData);
		}

		//Stream title only
		else if (rx_stream_title_only.indexIn(line) > -1) {
			QString title = rx_stream_title_only.cap(1);
			qDebug() << __FUNCTION__ << "Stream title:" << title;
			_mediaData.title = title;

			emit mediaDataChanged(_mediaData);
		}


		//The following things are not sent when the file has started to play
		//(or if sent, GUI will ignore anyway...)
		//So not process anymore, if video is playing to save some time
		if (_currentState == Phonon::PlayingState) {
			return;
		}
		///


		else if ((_mplayerVersion == MPlayerProcess::MPLAYER_VERSION_NOTFOUND) &&
				(line.startsWith("MPlayer "))) {
			_mplayerVersion = MPlayerVersion::parse(line);
		}

		//Stream name
		else if (rx_stream_name.indexIn(line) > -1) {
			QString name = rx_stream_name.cap(1);
			qDebug() << __FUNCTION__ << "Stream name:" << name;
			_mediaData.streamName = name;
		}

		//Stream genre
		else if (rx_stream_genre.indexIn(line) > -1) {
			QString genre = rx_stream_genre.cap(1);
			qDebug() << __FUNCTION__ << "Stream genre:" << genre;
			_mediaData.streamGenre = genre;
		}

		//Stream website
		else if (rx_stream_website.indexIn(line) > -1) {
			QString website = rx_stream_website.cap(1);
			qDebug() << __FUNCTION__ << "Stream website:" << website;
			if (!website.isEmpty() && website.at(website.size() - 1) == '/') {
				website.remove(website.size() - 1, 1);
			}
			_mediaData.streamWebsite = website;
		}

		//Subtitle
		//static QRegExp rx_subtitle("^ID_(SUBTITLE|FILE_SUB|VOBSUB)_ID=(\\d+)");
		else if (rx_subtitle.indexIn(line) > -1) {
			//DVD line example:

			//subtitle ( sid ): 1 language: en
			//ID_SUBTITLE_ID=1
			//ID_SID_1_LANG=en

			//subtitle ( sid ): 3 language: fr
			//ID_SUBTITLE_ID=3
			//ID_SID_3_LANG=fr

			//subtitle ( sid ): 4 language: unknown
			//ID_SUBTITLE_ID=4
			//Warning! ---> No ID_SID...

			//.str file example:
			//SUB: Detected subtitle file format: subviewer
			//SUB: Read 612 subtitles.
			//ID_FILE_SUB_ID=0
			//ID_FILE_SUB_FILENAME=D:/The Ring CD1.srt
			//SUB: Added subtitle file (1): D:/The Ring CD1.srt

			//Matroska line example:
			//ID_SUBTITLE_ID=0
			//ID_SID_0_NAME=Piste de présentation
			//[mkv] Track ID 4: subtitles (S_TEXT/UTF8) Piste de présentation, -sid 0, -slang mis
			//ID_SUBTITLE_ID=1
			//ID_SID_1_LANG=fre
			//[mkv] Track ID 5: subtitles (S_VOBSUB), -sid 1, -slang fre
			//ID_SUBTITLE_ID=2
			//ID_SID_2_LANG=eng

			int id = rx_subtitle.cap(2).toInt();
			const QString type = rx_subtitle.cap(1);

			if (type == "FILE_SUB") {
			} else if (type == "VOBSUB") {
			} else if (type == "SUBTITLE") {
			}
		}

		//Subtitle
		//static QRegExp rx_sid("^ID_(SID|VSID)_(\\d+)_(LANG|NAME)=(.*)");
		else if (rx_sid.indexIn(line) > -1) {
			int id = rx_sid.cap(2).toInt();
			const QString lang = rx_sid.cap(4);
			const QString attr = rx_sid.cap(3);
			const QString type = rx_sid.cap(1);
			qDebug() << __FUNCTION__ << "Subtitle id:" << id << "lang:" << lang << "type:" << type << "attr:" << attr;

			if (type == "VSID") {
			} else if (type == "SID") {
			}

			if (attr == "NAME") {
			} else if (attr == "LANG") {
			}

			emit subtitleAdded(id, lang, type);
		}

		//Subtitle
		//static QRegExp rx_subtitle_file("^SUB: Added subtitle file \\((\\d+)\\): (.*)");
		else if (rx_subtitle_file.indexIn(line) > -1) {
			//MPlayer make the id start at number 1,
			//we want it to start at number 0
			int id = rx_subtitle_file.cap(1).toInt() - 1;
			const QString fileName = rx_subtitle_file.cap(2);
			qDebug() << __FUNCTION__ << "Subtitle id:" << id << "file:" << fileName;

			emit subtitleAdded(id, fileName, "file");

			if (id == 0) {
				qDebug() << __FUNCTION__ << "Current subtitle changed:" << id;
				emit subtitleChanged(0);
			}
		}

		//AO
		else if (rx_ao.indexIn(line) > -1) {
			//emit receivedAO(rx_ao.cap(1));
		}

		//DVD and Matroska audio tracks
		else if (rx_audio_mat.indexIn(line) > -1) {
			//DVD examples:
			//ID_AUDIO_ID=129
			//ID_AID_129_LANG=fr
			//ID_AUDIO_ID=128
			//ID_AID_128_LANG=en

			//Matroska examples:
			//[mkv] Track ID 1: video (V_REAL/RV40), -vid 0
			//ID_AUDIO_ID=0
			//ID_AID_0_LANG=fre
			//[mkv] Track ID 3: audio (A_AAC/MPEG4/LC/SBR), -aid 1, -alang eng
			//ID_SUBTITLE_ID=0
			//ID_SID_0_NAME=Piste de présentation
			//[mkv] Track ID 4: subtitles (S_TEXT/UTF8) Piste de présentation, -sid 0, -slang mis
			//ID_SUBTITLE_ID=1
			//ID_SID_1_LANG=fre
			//[mkv] Track ID 5: subtitles (S_VOBSUB), -sid 1, -slang fre
			//ID_SUBTITLE_ID=2
			//ID_SID_2_LANG=eng
			//[mkv] Track ID 6: subtitles (S_VOBSUB), -sid 2, -slang eng
			//[mkv] Will play video track 1.
			//Matroska file format detected.
			//VIDEO:  [RV40]  704x296  24bpp  25.000 fps    0.0 kbps ( 0.0 kbyte/s)

			int id = rx_audio_mat.cap(1).toInt();
			QString lang = rx_audio_mat.cap(3);
			QString attr = rx_audio_mat.cap(2);
			qDebug() << __FUNCTION__ << "Audio id:" << id << "lang:" << lang << "attr:" << attr;

			if (attr == "NAME") {
				//lang=english, spanish...
				//_mediaData.audioTrackList.addName(id, lang);
			} else if (attr == "LANG") {
				//lang=en, fr...
				//_mediaData.audioTrackList.addLang(id, lang);
			}

			emit audioChannelAdded(id, lang);
		}

		//static QRegExp rx_mkvchapters("\\[mkv\\] Chapter (\\d+) from (.*) to (.*), (.*)");
		//Matroska chapters
		else if (rx_mkvchapters.indexIn(line) != -1) {
			//Examples:
			//[mkv] Chapter 0 from 00:10:09.800 to 00:00:00.000, La Cellule
			//[mkv] Chapter 1 from 00:15:02.080 to 00:00:00.000, Plus l'on Approche de César
			//[mkv] Chapter 2 from 00:19:16.400 to 00:00:00.000, La Compagnie Charlie
			//[mkv] Chapter 3 from 00:24:41.520 to 00:00:00.000, Guadalcanal

			int id = rx_mkvchapters.cap(1).toInt();
			QString from = rx_mkvchapters.cap(2);
			QString to = rx_mkvchapters.cap(3);
			QString title = rx_mkvchapters.cap(4);
			qDebug() << __FUNCTION__ << "mkv chapter:" << id << "title:" << title << "from:" << from << "to:" << to;

			emit mkvChapterAdded(id, title, from, to);
		}

		//VCD titles
		/*else if (rx_vcd.indexIn(line) > -1) {
			int ID = rx_vcd.cap(1).toInt();
			QString length = rx_vcd.cap(2);
			//_mediaData.titles.addID(ID);
			_mediaData.titles.addName(ID, length);
		}
		else

		//Audio CD titles
		if (rx_cdda.indexIn(line) > -1) {
			int ID = rx_cdda.cap(1).toInt();
			QString length = rx_cdda.cap(2);
			double duration = 0;
			QRegExp r("(\\d+):(\\d+):(\\d+)");
			if (r.indexIn(length) > -1) {
				duration = r.cap(1).toInt() * 60;
				duration += r.cap(2).toInt();
			}
			_mediaData.titles.addID(ID);
			//QString name = QString::number(ID) + " (" + length + ")";
			//_mediaData.titles.addName(ID, name);
			_mediaData.titles.addDuration(ID, duration);
		}
		else*/

		//DVD titles
		else if (rx_titles.indexIn(line) > -1) {
			//Available DVD titles
			int titles = rx_title.cap(1).toInt();
		}

		else if (rx_title.indexIn(line) > -1) {
			//DVD example:
			//ID_DVD_TITLES=8
			//ID_DVD_TITLE_1_CHAPTERS=2
			//ID_DVD_TITLE_1_ANGLES=1
			//ID_DVD_TITLE_2_CHAPTERS=12
			//ID_DVD_TITLE_2_ANGLES=1
			//ID_DVD_TITLE_1_LENGTH=18.560
			//ID_DVD_TITLE_2_LENGTH=5055.000
			//ID_DVD_DISC_ID=6B5CDFED561E882B949047C87A88BCB4
			//ID_DVD_CURRENT_TITLE=1

			int titleId = rx_title.cap(1).toInt();
			const QString attr = rx_title.cap(2);

			if (attr == "CHAPTERS") {
				int chapters = rx_title.cap(3).toInt();
				qDebug() << __FUNCTION__ << "DVD titleId:" << titleId << "chapters:" << chapters;

				emit chapterAdded(titleId, chapters);
			}

			else if (attr == "ANGLES") {
				int angles = rx_title.cap(3).toInt();
				qDebug() << __FUNCTION__ << "DVD titleId:" << titleId << "angles:" << angles;

				emit angleAdded(titleId, angles);
			}

			else if (attr == "LENGTH") {
				double length = rx_title.cap(3).toDouble();
				qDebug() << __FUNCTION__ << "DVD titleId:" << titleId << "length:" << length << "attr:" << attr;

				emit titleAdded(titleId, (int) (length * SECONDS_CONVERTION));
			}
		}

		//Creating index
		else if (rx_create_index.indexIn(line) > -1) {
			emit receivedCreatingIndex(line);
		}

		//Catch resolving message
		else if (rx_resolving.indexIn(line) > -1) {
			QString msg = rx_resolving.cap(1);
			qDebug() << __FUNCTION__ << "Resolving:" << msg;
			emit resolvingMessageReceived(msg);
		}

		//Catch resolving failed
		else if (rx_resolving_failed.indexIn(line) > -1) {
			_errorString = line;
			_errorType = Phonon::NormalError;
			changeState(Phonon::ErrorState);
		}

		//Catch connecting message
		else if (rx_connecting.indexIn(line) > -1) {
			QString msg = rx_connecting.cap(1);
			qDebug() << __FUNCTION__ << "Connecting:" << msg;
			emit connectingMessageReceived(msg);
			changeState(Phonon::BufferingState);
		}

		else if (rx_read_failed.indexIn(line) > -1) {
			_errorString = "Cannot read the network stream";
			_errorType = Phonon::NormalError;
			changeState(Phonon::ErrorState);
		}

		//Catch cache messages
		else if (rx_cache_fill.indexIn(line) > -1) {
			QString tmp = rx_cache_fill.cap(1);
			tmp = tmp.trimmed();
			float percentFilled = tmp.toFloat();
			qDebug() << __FUNCTION__ << "Cache %:" << percentFilled << tmp;
			emit bufferStatus(percentFilled);
		}

		//Meta data infos

		//Title
		else if (rx_clip_title.indexIn(line) > -1) {
			QString title = rx_clip_title.cap(2);
			qDebug() << __FUNCTION__ << "Clip title:" << title;
			_mediaData.title = title;
		}

		//Artist
		else if (rx_clip_artist.indexIn(line) > -1) {
			QString artist = rx_clip_artist.cap(1);
			qDebug() << __FUNCTION__ << "Clip artist:" << artist;
			_mediaData.artist = artist;
		}

		//Author
		else if (rx_clip_author.indexIn(line) > -1) {
			QString author = rx_clip_author.cap(1);
			qDebug() << __FUNCTION__ << "Clip author:" << author;
			_mediaData.author = author;
		}

		//Album
		else if (rx_clip_album.indexIn(line) > -1) {
			QString album = rx_clip_album.cap(1);
			qDebug() << __FUNCTION__ << "Clip album:" << album;
			_mediaData.album = album;
		}

		//Genre
		else if (rx_clip_genre.indexIn(line) > -1) {
			QString genre = rx_clip_genre.cap(1);
			qDebug() << __FUNCTION__ << "Clip genre:" << genre;
			_mediaData.genre = genre;
		}

		//Date
		else if (rx_clip_date.indexIn(line) > -1) {
			QString date = rx_clip_date.cap(2);
			qDebug() << __FUNCTION__ << "Clip date:" << date;
			_mediaData.date = date;
		}

		//Track
		else if (rx_clip_track.indexIn(line) > -1) {
			QString track = rx_clip_track.cap(1);
			qDebug() << __FUNCTION__ << "Clip track:" << track;
			_mediaData.track = track;
		}

		//Copyright
		else if (rx_clip_copyright.indexIn(line) > -1) {
			QString copyright = rx_clip_copyright.cap(1);
			qDebug() << __FUNCTION__ << "Clip copyright:" << copyright;
			_mediaData.copyright = copyright;
		}

		//Comment
		else if (rx_clip_comment.indexIn(line) > -1) {
			QString comment = rx_clip_comment.cap(1);
			qDebug() << __FUNCTION__ << "Clip comment:" << comment;
			_mediaData.comment = comment;
		}

		//Software
		else if (rx_clip_software.indexIn(line) > -1) {
			QString software = rx_clip_software.cap(1);
			qDebug() << __FUNCTION__ << "Clip software:" << software;
			_mediaData.software = software;
		}

		//Catch "Starting playback..." message
		else if (rx_play.indexIn(line) > -1) {
			//OK, now all the media datas should be in clean state
			//Second time we emit mediaLoaded(), this one is usefull for DVD with angles/chapters/subtitles...
			//This must be changed, see MediaObject::mediaLoaded()
			emit mediaLoaded();
			emit mediaDataChanged(_mediaData);

			if (_mediaData.hasVideo) {
				//If we have a video to display, wait for getting the video size
				//before to be in PlayingState
				//This is a bugfix for mediaplayer example from Trolltech
			} else {
				//For audio streams, it's ok we are in PlayingState
				changeState(Phonon::PlayingState);
			}
		}

		//Generic things
		if (rx_generic.indexIn(line) > -1) {
			const QString tag = rx_generic.cap(1);
			const QString value = rx_generic.cap(2);

			if (tag == "ID_VIDEO_ID") {
				//First string to tell us that the media contains a video track
				_mediaData.hasVideo = true;
				qDebug() << __FUNCTION__ << "Video:" << _mediaData.hasVideo;
				emit hasVideoChanged(_mediaData.hasVideo);
			}

			else if (tag == "ID_AUDIO_ID") {
				//First string to tell us that the media contains an audio track
				/*int audioId = value.toInt();
				qDebug() << __FUNCTION__ << "ID_AUDIO_ID:" << audioId;
				md.audios.addID(audioId);*/
			}

			else if (tag == "ID_LENGTH") {
				_mediaData.totalTime = (qint64) (value.toDouble() * SECONDS_CONVERTION);
				qDebug() << __FUNCTION__ << "Media total time:" << _mediaData.totalTime;
				emit totalTimeChanged(_mediaData.totalTime);
			}

			else if (tag == "ID_SEEKABLE") {
				_mediaData.isSeekable = value.toInt();
				qDebug() << __FUNCTION__ << "Media seekable:" << _mediaData.isSeekable;
				emit seekableChanged(_mediaData.isSeekable);
			}

			else if (tag == "ID_VIDEO_WIDTH") {
				_mediaData.videoWidth = value.toInt();
				qDebug() << __FUNCTION__ << "Video width:" << _mediaData.videoWidth;
			}

			else if (tag == "ID_VIDEO_HEIGHT") {
				_mediaData.videoHeight = value.toInt();
				qDebug() << __FUNCTION__ << "Video height:" << _mediaData.videoHeight;
			}

			else if (tag == "ID_VIDEO_ASPECT") {
				_mediaData.videoAspectRatio = value.toDouble();
				if (_mediaData.videoAspectRatio == 0.0) {
					//I hope width & height are already set
					_mediaData.videoAspectRatio = (double) _mediaData.videoWidth / _mediaData.videoHeight;
				}
				qDebug() << __FUNCTION__ << "Video aspect:" << _mediaData.videoAspectRatio;
			}

			else if (tag == "ID_DVD_DISC_ID") {
				//_mediaData.dvd_id = value;
				qDebug() << __FUNCTION__ << "DVD disc Id:" << value;
			}

			else if (tag == "ID_DEMUXER") {
				_mediaData.demuxer = value;
			}

			else if (tag == "ID_VIDEO_FORMAT") {
				_mediaData.videoFormat = value;
			}

			else if (tag == "ID_AUDIO_FORMAT") {
				_mediaData.audioFormat = value;
			}

			else if (tag == "ID_VIDEO_BITRATE") {
				_mediaData.videoBitrate = value.toInt();
			}

			else if (tag == "ID_VIDEO_FPS") {
				_mediaData.videoFPS = value.toDouble();
				qDebug() << __FUNCTION__ << "Video FPS:" << _mediaData.videoFPS;
			}

			else if (tag == "ID_AUDIO_BITRATE") {
				_mediaData.audioBitrate = value.toInt();
			}

			else if (tag == "ID_AUDIO_RATE") {
				_mediaData.audioRate = value.toInt();
			}

			else if (tag == "ID_AUDIO_NCH") {
				_mediaData.audioNbChannels = value.toInt();
			}

			else if (tag == "ID_VIDEO_CODEC") {
				_mediaData.videoCodec = value;
			}

			else if (tag == "ID_AUDIO_CODEC") {
				_mediaData.audioCodec = value;
			}

			else if (tag == "ID_DVD_CURRENT_TITLE") {
				int id = value.toInt();
				if (_currentTitleId != id) {
					_currentTitleId = id;
					emit titleChanged(_currentTitleId);
				}
			}
		}
	}
}

void MPlayerProcess::finished(int exitCode, QProcess::ExitStatus exitStatus) {
	if (exitCode != 0) {
		qCritical() << __FUNCTION__ << "Error: MPlayer crashed";
		_errorString = "MPlayer crashed";
		_errorType = Phonon::FatalError;
		changeState(Phonon::ErrorState);
	} else {
		//exitCode == 0 means everything is OK

		switch (exitStatus) {
		case QProcess::NormalExit:
			qDebug() << __FUNCTION__ << "MPlayer process exited normally";
			changeState(Phonon::StoppedState);
			if (_endOfFileReached) {
				emit endOfFileReached();
			}
			break;
		case QProcess::CrashExit:
			qCritical() << __FUNCTION__ << "Error: MPlayer process crashed";
			_errorString = "MPlayer process crashed";
			_errorType = Phonon::FatalError;
			changeState(Phonon::ErrorState);
			break;
		default:
			qCritical() << __FUNCTION__ << "Error: unknown state:" << exitStatus;
			return;
		}
	}
}

void MPlayerProcess::changeState(Phonon::State newState) {
	_previousState = _currentState;
	_currentState = newState;

	if (_currentState != _previousState) {
		emit stateChanged(_currentState, _previousState);
	}
}

Phonon::State MPlayerProcess::currentState() const {
	return _currentState;
}

Phonon::State MPlayerProcess::previousState() const {
	return _previousState;
}

void MPlayerProcess::error(QProcess::ProcessError error) {
	_errorType = Phonon::FatalError;

	switch (error) {
	case QProcess::FailedToStart:
		_errorString = "MPlayer failed to start: either MPlayer is missing, or you may have insufficient permissions";
		break;
	case QProcess::Crashed:
		_errorString = "MPlayer crashed some time after starting successfully";
		break;
	case QProcess::Timedout:
		_errorString = "MPlayer: QProcess::waitFor() function timed out";
		break;
	case QProcess::WriteError:
		_errorString = "An error occurred when attempting to write to MPlayer."
					"For example, MPlayer may not be running, or it may have closed its input channel";
		break;
	case QProcess::ReadError:
		_errorString = "An error occurred when attempting to read from MPlayer."
					"For example, the process may not be running";
		break;
	case QProcess::UnknownError:
		_errorString = "An unknown error occurred";
		break;
	default:
		qCritical() << __FUNCTION__ << "Error: unknown error number:" << error;
	}

	changeState(Phonon::ErrorState);
}
