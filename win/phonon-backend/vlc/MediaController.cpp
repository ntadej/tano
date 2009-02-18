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

#include "MediaController.h"

namespace Phonon
{
namespace VLC_MPlayer
{

MediaController::MediaController() {
	clearMediaController();
}

MediaController::~MediaController() {
}

void MediaController::clearMediaController() {
	_currentAudioChannel = Phonon::AudioChannelDescription();
	_availableAudioChannels.clear();

	_currentSubtitle = Phonon::SubtitleDescription();
	_availableSubtitles.clear();

	_currentAngle = 0;
	_availableAngles = 0;

#ifdef NEW_TITLE_CHAPTER_HANDLING
	_currentChapter = Phonon::ChapterDescription();
	_availableChapters.clear();
#else
	_currentChapter = 0;
	_availableChapters = 0;
#endif	//NEW_TITLE_CHAPTER_HANDLING

#ifdef NEW_TITLE_CHAPTER_HANDLING
	_currentTitle = Phonon::TitleDescription();
	_availableTitles.clear();
#else
	_currentTitle = 0;
	_availableTitles = 0;
#endif	//NEW_TITLE_CHAPTER_HANDLING

	_autoplayTitles = false;
}

bool MediaController::hasInterface(Interface iface) const {
	switch (iface) {
	case AddonInterface::NavigationInterface:
		return true;
		break;
	case AddonInterface::ChapterInterface:
		return true;
		break;
	case AddonInterface::AngleInterface:
		return true;
		break;
	case AddonInterface::TitleInterface:
		return true;
		break;
	case AddonInterface::SubtitleInterface:
		return true;
		break;
	case AddonInterface::AudioChannelInterface:
		return true;
		break;
	default:
		qCritical() << __FUNCTION__ << "Error: unsupported AddonInterface::Interface" << iface;
	}

	return false;
}

QVariant MediaController::interfaceCall(Interface iface, int command, const QList<QVariant> & arguments) {
	switch (iface) {

	case AddonInterface::ChapterInterface:
		switch (static_cast<AddonInterface::ChapterCommand>(command)) {

#ifdef NEW_TITLE_CHAPTER_HANDLING
			case AddonInterface::availableChapters:
				return QVariant::fromValue(availableChapters());
#else
			case AddonInterface::availableChapters:
				return availableChapters();
#endif	//NEW_TITLE_CHAPTER_HANDLING

#ifdef NEW_TITLE_CHAPTER_HANDLING
			case AddonInterface::currentChapter:
				return QVariant::fromValue(currentChapter());
#else
			case AddonInterface::chapter:
				return currentChapter();
#endif	//NEW_TITLE_CHAPTER_HANDLING

#ifdef NEW_TITLE_CHAPTER_HANDLING
			case AddonInterface::setCurrentChapter:
				if (arguments.isEmpty() || !arguments.first().canConvert<ChapterDescription>()) {
					qCritical() << __FUNCTION__ << "Error: arguments invalid";
					return false;
				}
				setCurrentChapter(arguments.first().value<ChapterDescription>());
				return true;
#else
			case AddonInterface::setChapter:
				if (arguments.isEmpty() || !arguments.first().canConvert(QVariant::Int)) {
					qCritical() << __FUNCTION__ << "Error: arguments invalid";
					return false;
				}
				setCurrentChapter(arguments.first().toInt());
				return true;
#endif	//NEW_TITLE_CHAPTER_HANDLING

			default:
				qCritical() << __FUNCTION__ << "Error: unsupported AddonInterface::ChapterInterface command:" << command;
		}
		break;

	case AddonInterface::TitleInterface:
		switch (static_cast<AddonInterface::TitleCommand>(command)) {

#ifdef NEW_TITLE_CHAPTER_HANDLING
			case AddonInterface::availableTitles:
				return QVariant::fromValue(availableTitles());
#else
			case AddonInterface::availableTitles:
				return availableTitles();
#endif	//NEW_TITLE_CHAPTER_HANDLING

#ifdef NEW_TITLE_CHAPTER_HANDLING
			case AddonInterface::currentTitle:
				return QVariant::fromValue(currentTitle());
#else
			case AddonInterface::title:
				return currentTitle();
#endif	//NEW_TITLE_CHAPTER_HANDLING

#ifdef NEW_TITLE_CHAPTER_HANDLING
			case AddonInterface::setCurrentTitle:
				if (arguments.isEmpty() || !arguments.first().canConvert<TitleDescription>()) {
					qCritical() << __FUNCTION__ << "Error: arguments invalid";
					return false;
				}
				setCurrentTitle(arguments.first().value<TitleDescription>());
				return true;
#else
			case AddonInterface::setTitle:
				if (arguments.isEmpty() || !arguments.first().canConvert(QVariant::Int)) {
					qCritical() << __FUNCTION__ << "Error: arguments invalid";
					return false;
				}
				setCurrentTitle(arguments.first().toInt());
				return true;
#endif	//NEW_TITLE_CHAPTER_HANDLING

			case AddonInterface::autoplayTitles:
				return autoplayTitles();
			case AddonInterface::setAutoplayTitles:
				if (arguments.isEmpty() || !arguments.first().canConvert(QVariant::Bool)) {
					qCritical() << __FUNCTION__ << "Error: arguments invalid";
					return false;
				}
				setAutoplayTitles(arguments.first().toBool());
				return true;
			default:
				qCritical() << __FUNCTION__ << "Error: unsupported AddonInterface::TitleInterface command:" << command;
		}
		break;

	case AddonInterface::AngleInterface:
		switch (static_cast<AddonInterface::AngleCommand>(command)) {
			case AddonInterface::availableAngles:
			case AddonInterface::angle:
			case AddonInterface::setAngle:
				break;
			default:
				qCritical() << __FUNCTION__ << "Error: unsupported AddonInterface::AngleInterface command:" << command;
		}
		break;

	case AddonInterface::SubtitleInterface:
		switch (static_cast<AddonInterface::SubtitleCommand>(command)) {
			case AddonInterface::availableSubtitles:
				return QVariant::fromValue(availableSubtitles());

			case AddonInterface::currentSubtitle:
				return QVariant::fromValue(currentSubtitle());

			case AddonInterface::setCurrentSubtitle:
				if (arguments.isEmpty() || !arguments.first().canConvert<SubtitleDescription>()) {
					qCritical() << __FUNCTION__ << "Error: arguments invalid";
					return false;
				}
				setCurrentSubtitle(arguments.first().value<SubtitleDescription>());
				return true;

			default:
				qCritical() << __FUNCTION__ << "Error: unsupported AddonInterface::SubtitleInterface command:" << command;
		}
		break;

	case AddonInterface::AudioChannelInterface:
		switch (static_cast<AddonInterface::AudioChannelCommand>(command)) {

			case AddonInterface::availableAudioChannels:
				return QVariant::fromValue(availableAudioChannels());

			case AddonInterface::currentAudioChannel:
				return QVariant::fromValue(currentAudioChannel());

			case AddonInterface::setCurrentAudioChannel:
				if (arguments.isEmpty() || !arguments.first().canConvert<AudioChannelDescription>()) {
					qCritical() << __FUNCTION__ << "Error: arguments invalid";
					return false;
				}
				setCurrentAudioChannel(arguments.first().value<AudioChannelDescription>());
				return true;

			default:
				qCritical() << __FUNCTION__ << "Error: unsupported AddonInterface::AudioChannelInterface command:" << command;
		}
		break;

	default:
		qCritical() << __FUNCTION__ << "Error: unsupported AddonInterface::Interface:" << iface;
	}

	return new QVariant();
}

}}	//Namespace Phonon::VLC_MPlayer
