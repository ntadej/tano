/*
 * MPlayer backend for the Phonon library
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

#ifndef PHONON_MPLAYER_MEDIACONTROLLER_H
#define PHONON_MPLAYER_MEDIACONTROLLER_H

#include "config.h"

#include <phonon/addoninterface.h>
#include <phonon/objectdescription.h>

#include <QtCore/QObject>

class MPlayerProcess;

namespace Phonon
{
namespace MPlayer
{

static const int MPLAYER_DEFAULT_DVD_TITLE = 1;
static const int MPLAYER_DEFAULT_VCD_TITLE = MPLAYER_DEFAULT_DVD_TITLE;
static const int MPLAYER_DEFAULT_CDAUDIO_TITLE = MPLAYER_DEFAULT_DVD_TITLE;

/**
 * Interface for AddonInterface.
 *
 * Normally MediaObject inherits directly from AddonInterface, but wait, I don't want
 * 3000LOC classes...
 *
 * @see MediaObject
 * @author Tanguy Krotoff
 */
class MediaController : public QObject, public AddonInterface {
	Q_OBJECT
public:

	MediaController(QObject * parent);

	virtual ~MediaController();

	bool hasInterface(Interface iface) const;

	QVariant interfaceCall(Interface iface, int command, const QList<QVariant> & arguments = QList<QVariant>());

signals:

	//MediaController signals
	void availableSubtitlesChanged();
	void availableAudioChannelsChanged();

#ifdef NEW_TITLE_CHAPTER_HANDLING
	void availableChaptersChanged();
	void availableTitlesChanged();
#else
	void availableChaptersChanged(int availableChapters);
	void availableTitlesChanged(int availableTitles);
#endif	//NEW_TITLE_CHAPTER_HANDLING

	void availableAnglesChanged(int availableAngles);

	//FIXME Won't be emitted due to MPlayer, no signal from MPlayerProcess
	void angleChanged(int angleNumber);

	//FIXME Won't be emitted due to MPlayer, no signal from MPlayerProcess
	void chapterChanged(int chapterNumber);

	void titleChanged(int titleNumber);

private slots:

	void audioChannelAdded(int id, const QString & lang);
	void subtitleAdded(int id, const QString & name, const QString & type);
	void subtitleChanged(int id);
	void titleAdded(int id, qint64 length);
	void chapterAdded(int titleId, int chapters);
	void mkvChapterAdded(int id, const QString & title, const QString & from, const QString & to);
	void angleAdded(int titleId, int angles);

protected:

	/**
	 * Clear all (i.e availableSubtitles, availableChapters...).
	 *
	 * This is used each time we restart the video.
	 */
	void clearMediaController();

	/** MPlayer process. */
	MPlayerProcess * _process;

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

private:

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

	void loadSubtitleFile(const QString & filename);

	static QString convertMilliseconds(qint64 milliseconds);
};

}}	//Namespace Phonon::MPlayer

#endif	//PHONON_MPLAYER_MEDIACONTROLLER_H
