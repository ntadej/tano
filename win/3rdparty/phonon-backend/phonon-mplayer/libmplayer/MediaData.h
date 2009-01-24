/*
 * MPlayer backend for the Phonon library
 * Copyright (C) 2006-2008  Ricardo Villalba <rvm@escomposlinux.org>
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

#ifndef MEDIADATA_H
#define MEDIADATA_H

//For WId
#include <QtGui/QWidget>

#include <QtCore/QString>

/**
 * Stores some volatile informations about the stream/media/file.
 *
 * Contains the file meta data + others informations.
 *
 * Stores informations given by MPlayer using this command:
 * <pre>mplayer -identify -noquiet -frames 0 FILENAME</pre>
 *
 * @see http://xiph.org/vorbis/doc/Vorbis_I_spec.html#vorbis-spec-comment
 */
class MediaData {
public:

	MediaData();
	~MediaData();

	/** Reset all datas to zero. */
	void clear();

	/** Filename of the media. Full path. */
	QString filename;

	/**
	 * Media total time (duration) in milliseconds.
	 *
	 * ID_LENGTH. Example: 62.73
	 *
	 * FIXME
	 * MPlayer does not read VBR mp3 headers properly so
	 * ID_AUDIO_BITRATE + ID_LENGTH might be wrong
	 * cf http://article.gmane.org/gmane.comp.video.mplayer.devel/42003
	 * It's OK with CBR mp3.
	 *
	 * See http://bugzilla.mplayerhq.hu/show_bug.cgi?id=30
	 * See http://bugzilla.mplayerhq.hu/show_bug.cgi?id=465
	 */
	qint64 totalTime;

	/**
	 * Current playing time of the media in milliseconds.
	 *
	 * Updated each time MPlayer plays the media.
	 *
	 * If MPlayer is not playing, then the value is the last one known
	 */
	qint64 currentTime;

	/** Id of the widget where MPlayer will show the video. */
	WId videoWidgetId;

	//Resolution of the video

	/** ID_VIDEO_WIDTH. Example: 576 */
	int videoWidth;

	/** ID_VIDEO_HEIGHT. Example: 320 */
	int videoHeight;

	/**
	 * ID_VIDEO_ASPECT.
	 *
	 * MPlayer can return 0, then it is computed from videoWidth and videoHeight
	 *
	 * Example: 1.8000
	 */
	double videoAspectRatio;

	///

	/** ID_SEEKABLE. Example: 1 */
	bool isSeekable;

	/**
	 * If the stream contains a video or not.
	 *
	 * @see Phonon::MediaObject::hasVideoChanged()
	 * @see MPlayerProcess::hasVideoChanged()
	 */
	bool hasVideo;

	//Meta data infos
	QString title;
	QString artist;
	QString author;
	QString album;
	QString genre;
	QString date;
	QString track;
	QString copyright;
	QString comment;
	QString software;

	//Radio streaming infos
	QString streamName;
	QString streamGenre;
	QString streamWebsite;
	QString streamUrl;

	//Other datas not really useful for us,
	//just to show info to the user
	//Please try to respect the declarations order: should match MPlayer stdout

	/** ID_DEMUXER. Example: avi */
	QString demuxer;

	/** ID_VIDEO_FORMAT. Example: XVID */
	QString videoFormat;

	/** ID_VIDEO_BITRATE (bps). Example: 1013968 */
	int videoBitrate;

	//ID_VIDEO_WIDTH

	//ID_VIDEO_HEIGHT

	/** ID_VIDEO_FPS. Example: 29.970 */
	double videoFPS;

	//ID_VIDEO_ASPECT

	/** ID_AUDIO_FORMAT. Example: 85 */
	QString audioFormat;

	/**
	 * ID_AUDIO_BITRATE (bps).
	 *
	 * Example: 128000
	 */
	int audioBitrate;

	/** ID_AUDIO_RATE (Hz). Example: 44100 */
	int audioRate;

	/** ID_AUDIO_NCH. Example: 2 I don't know what it is, number of channels? */
	int audioNbChannels;

	//ID_LENGTH

	/** ID_VIDEO_CODEC. Example: ffodivx */
	QString videoCodec;

	/** ID_AUDIO_CODEC. Example: mad */
	QString audioCodec;
};

#endif	//MEDIADATA_H
