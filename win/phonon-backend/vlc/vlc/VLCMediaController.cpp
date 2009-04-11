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

#include "VLCMediaController.h"

#include "vlc_loader.h"
#include "vlc_symbols.h"

namespace Phonon
{
namespace VLC_Backend
{

VLCMediaController::VLCMediaController()
	: MediaController() {

	_vlcMediaPlayer = NULL;
}

VLCMediaController::~VLCMediaController() {
}

//AudioChannel
void VLCMediaController::setCurrentAudioChannel(const Phonon::AudioChannelDescription & audioChannel) {
	qDebug() << __FUNCTION__;

	_currentAudioChannel = audioChannel;
}

QList<Phonon::AudioChannelDescription> VLCMediaController::availableAudioChannels() const {
	return _availableAudioChannels;
}

Phonon::AudioChannelDescription VLCMediaController::currentAudioChannel() const {
	return _currentAudioChannel;
}

//Subtitle
void VLCMediaController::setCurrentSubtitle(const Phonon::SubtitleDescription & subtitle) {
	qDebug() << __FUNCTION__;

	_currentSubtitle = subtitle;
	int id = _currentSubtitle.index();
	QString type = _currentSubtitle.property("type").toString();

	if (type == "file") {
		QString filename = _currentSubtitle.property("name").toString();
		if (!filename.isEmpty()) {
			p_libvlc_video_set_subtitle_file(_vlcMediaPlayer, filename.toAscii().data(), _vlcException);

			//There is no subtitle event inside libvlc
			//so let's send our own event...
			_availableSubtitles << _currentSubtitle;
			emit availableSubtitlesChanged();
		}
	}
}

QList<Phonon::SubtitleDescription> VLCMediaController::availableSubtitles() const {
	return _availableSubtitles;
}

Phonon::SubtitleDescription VLCMediaController::currentSubtitle() const {
	return _currentSubtitle;
}

//Title

#ifdef NEW_TITLE_CHAPTER_HANDLING

void VLCMediaController::setCurrentTitle(const Phonon::TitleDescription & title) {
	_currentTitle = title;
}

QList<Phonon::TitleDescription> VLCMediaController::availableTitles() const {
	return _availableTitles;
}

Phonon::TitleDescription VLCMediaController::currentTitle() const {
	return _currentTitle;
}

#else

void VLCMediaController::setCurrentTitle(int titleNumber) {
	_currentTitle = titleNumber;
}

int VLCMediaController::availableTitles() const {
	return _availableTitles;
}

int VLCMediaController::currentTitle() const {
	return _currentTitle;
}

#endif	//NEW_TITLE_CHAPTER_HANDLING

void VLCMediaController::setAutoplayTitles(bool autoplay) {
	_autoplayTitles = autoplay;
}

bool VLCMediaController::autoplayTitles() const {
	return _autoplayTitles;
}

//Chapter

#ifdef NEW_TITLE_CHAPTER_HANDLING

void VLCMediaController::setCurrentChapter(const Phonon::ChapterDescription & chapter) {
	_currentChapter = chapter;
}

QList<Phonon::ChapterDescription> VLCMediaController::availableChapters() const {
	return _availableChapters;
}

Phonon::ChapterDescription VLCMediaController::currentChapter() const {
	return _currentChapter;
}

#else

void VLCMediaController::setCurrentChapter(int chapterNumber) {
}

int VLCMediaController::availableChapters() const {
	return _availableChapters;
}

int VLCMediaController::currentChapter() const {
	return _currentChapter;
}

#endif	//NEW_TITLE_CHAPTER_HANDLING

//Angle
void VLCMediaController::setCurrentAngle(int angleNumber) {
}

int VLCMediaController::availableAngles() const {
	return _availableAngles;
}

int VLCMediaController::currentAngle() const {
	return _currentAngle;
}

}}	//Namespace Phonon::VLC_Backend
