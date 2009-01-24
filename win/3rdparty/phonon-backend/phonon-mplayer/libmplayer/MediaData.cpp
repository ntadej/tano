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

#include "MediaData.h"

#include <QtCore/QtDebug>

MediaData::MediaData() {
	clear();
}

MediaData::~MediaData() {
}

void MediaData::clear() {
	filename.clear();

	totalTime = 0;
	currentTime = 0;

	videoWidgetId = 0;

	//FIXME Change it to true as MPlayer-1.0rc2 does not come
	//with ID_SEEKABLE (appeared in march 2008)
	//Reasonable to say that most of the media are seekable
	isSeekable = true;

	hasVideo = false;

	videoWidth = 0;
	videoHeight = 0;
	videoAspectRatio = (double) 4 / 3;

	title.clear();
	artist.clear();
	author.clear();
	album.clear();
	genre.clear();
	date.clear();
	track.clear();
	copyright.clear();
	comment.clear();
	software.clear();

	streamName.clear();
	streamGenre.clear();
	streamWebsite.clear();
	streamUrl.clear();

	demuxer.clear();
	videoFormat.clear();
	audioFormat.clear();
	videoBitrate = 0;
	videoFPS = 0;
	audioBitrate = 0;
	audioRate = 0;
	audioNbChannels = 0;
	videoCodec.clear();
	audioCodec.clear();
}
