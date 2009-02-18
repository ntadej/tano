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

#ifndef PHONON_VLC_MPLAYER_VLCMEDIACONTROLLER_H
#define PHONON_VLC_MPLAYER_VLCMEDIACONTROLLER_H

#include "../MediaController.h"

#include <vlc/vlc.h>

namespace Phonon
{
namespace VLC_MPlayer
{

/**
 * MediaController specific code for VLC.
 *
 * @author Tanguy Krotoff
 */
class VLCMediaController : public MediaController {
public:

	VLCMediaController();
	virtual ~VLCMediaController();

protected:

	//AudioChannel
	void setCurrentAudioChannel(const Phonon::AudioChannelDescription & audioChannel);
	QList<Phonon::AudioChannelDescription> availableAudioChannels() const;
	Phonon::AudioChannelDescription currentAudioChannel() const;

	//Subtitle
	void setCurrentSubtitle(const Phonon::SubtitleDescription & subtitle);
	QList<Phonon::SubtitleDescription> availableSubtitles() const;
	Phonon::SubtitleDescription currentSubtitle() const;

	//Angle
	void setCurrentAngle(int angleNumber);
	int availableAngles() const;
	int currentAngle() const;

#ifdef NEW_TITLE_CHAPTER_HANDLING
	//Chapter
	void setCurrentChapter(const Phonon::ChapterDescription & chapter);
	QList<Phonon::ChapterDescription> availableChapters() const;
	Phonon::ChapterDescription currentChapter() const;
#else
	//Chapter
	void setCurrentChapter(int chapterNumber);
	int availableChapters() const;
	int currentChapter() const;
#endif	//NEW_TITLE_CHAPTER_HANDLING

#ifdef NEW_TITLE_CHAPTER_HANDLING
	//Title
	void setCurrentTitle(const Phonon::TitleDescription & title);
	QList<Phonon::TitleDescription> availableTitles() const;
	Phonon::TitleDescription currentTitle() const;
#else
	//Title
	void setCurrentTitle(int titleNumber);
	int availableTitles() const;
	int currentTitle() const;
#endif	//NEW_TITLE_CHAPTER_HANDLING

	void setAutoplayTitles(bool autoplay);
	bool autoplayTitles() const;

	//MediaPlayer
	libvlc_media_player_t * _vlcMediaPlayer;

private:
};

}}	//Namespace Phonon::VLC_MPlayer

#endif	//PHONON_VLC_MPLAYER_VLCMEDIACONTROLLER_H
