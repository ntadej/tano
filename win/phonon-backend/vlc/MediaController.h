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

#ifndef PHONON_VLC_MPLAYER_MEDIACONTROLLER_H
#define PHONON_VLC_MPLAYER_MEDIACONTROLLER_H

#include "config.h"

#include <phonon/addoninterface.h>
#include <phonon/objectdescription.h>

namespace Phonon
{
namespace VLC_MPlayer
{

/**
 * Interface for AddonInterface.
 *
 * This class exists only for code factorization between VLC and MPlayer backends.
 * Normally MediaObject inherits directly from AddonInterface, but wait, I don't want
 * 3000LOC classes...
 *
 * This class cannot inherit from QObject has MediaObject already inherit from QObject.
 * This is a Qt limitation: there is no possibility to inherit virtual Qobject :/
 * See http://doc.trolltech.com/qq/qq15-academic.html
 * Phonon implementation got the same problem.
 *
 * @see VLCMediaController
 * @see MPlayerMediaController
 * @see VLCMediaObject
 * @see MPlayerMediaObject
 * @see MediaObject
 * @author Tanguy Krotoff
 */
class MediaController : public AddonInterface {
public:

	MediaController();
	virtual ~MediaController();

	bool hasInterface(Interface iface) const;

	QVariant interfaceCall(Interface iface, int command, const QList<QVariant> & arguments = QList<QVariant>());

	//MediaController signals
	virtual void availableSubtitlesChanged() = 0;
	virtual void availableAudioChannelsChanged() = 0;

#ifdef NEW_TITLE_CHAPTER_HANDLING
	virtual void availableChaptersChanged() = 0;
	virtual void availableTitlesChanged() = 0;
#else
	virtual void availableChaptersChanged(int availableChapters) = 0;
	virtual void availableTitlesChanged(int availableTitles) = 0;
#endif	//NEW_TITLE_CHAPTER_HANDLING

	virtual void availableAnglesChanged(int availableAngles) = 0;
	virtual void angleChanged(int angleNumber) = 0;
	virtual void chapterChanged(int chapterNumber) = 0;
	virtual void titleChanged(int titleNumber) = 0;

protected:

	//AudioChannel
	virtual void setCurrentAudioChannel(const Phonon::AudioChannelDescription & audioChannel) = 0;
	virtual QList<Phonon::AudioChannelDescription> availableAudioChannels() const = 0;
	virtual Phonon::AudioChannelDescription currentAudioChannel() const = 0;

	//Subtitle
	virtual void setCurrentSubtitle(const Phonon::SubtitleDescription & subtitle) = 0;
	virtual QList<Phonon::SubtitleDescription> availableSubtitles() const = 0;
	virtual Phonon::SubtitleDescription currentSubtitle() const = 0;

	//Angle
	virtual void setCurrentAngle(int angleNumber) = 0;
	virtual int availableAngles() const = 0;
	virtual int currentAngle() const = 0;

#ifdef NEW_TITLE_CHAPTER_HANDLING
	//Chapter
	virtual void setCurrentChapter(const Phonon::ChapterDescription & chapter) = 0;
	virtual QList<Phonon::ChapterDescription> availableChapters() const = 0;
	virtual Phonon::ChapterDescription currentChapter() const = 0;
#else
	//Chapter
	virtual void setCurrentChapter(int chapterNumber) = 0;
	virtual int availableChapters() const = 0;
	virtual int currentChapter() const = 0;
#endif	//NEW_TITLE_CHAPTER_HANDLING

#ifdef NEW_TITLE_CHAPTER_HANDLING
	//Title
	virtual void setCurrentTitle(const Phonon::TitleDescription & title) = 0;
	virtual QList<Phonon::TitleDescription> availableTitles() const = 0;
	virtual Phonon::TitleDescription currentTitle() const = 0;
#else
	//Title
	virtual void setCurrentTitle(int titleNumber) = 0;
	virtual int availableTitles() const = 0;
	virtual int currentTitle() const = 0;
#endif	//NEW_TITLE_CHAPTER_HANDLING

	virtual void setAutoplayTitles(bool autoplay) = 0;
	virtual bool autoplayTitles() const = 0;

	/**
	 * Clear all (i.e availableSubtitles, availableChapters...).
	 *
	 * This is used each time we restart the video.
	 */
	virtual void clearMediaController();

	Phonon::AudioChannelDescription _currentAudioChannel;
	QList<Phonon::AudioChannelDescription> _availableAudioChannels;

	Phonon::SubtitleDescription _currentSubtitle;
	QList<Phonon::SubtitleDescription> _availableSubtitles;

	int _currentAngle;
	int _availableAngles;

#ifdef NEW_TITLE_CHAPTER_HANDLING
	Phonon::ChapterDescription _currentChapter;
	QList<Phonon::ChapterDescription> _availableChapters;
#else
	int _currentChapter;
	int _availableChapters;
#endif	//NEW_TITLE_CHAPTER_HANDLING

#ifdef NEW_TITLE_CHAPTER_HANDLING
	Phonon::TitleDescription _currentTitle;
	QList<Phonon::TitleDescription> _availableTitles;
#else
	int _currentTitle;
	int _availableTitles;
#endif	//NEW_TITLE_CHAPTER_HANDLING

	bool _autoplayTitles;

private:
};

}}	//Namespace Phonon::VLC_MPlayer

#endif	//PHONON_VLC_MPLAYER_MEDIACONTROLLER_H
