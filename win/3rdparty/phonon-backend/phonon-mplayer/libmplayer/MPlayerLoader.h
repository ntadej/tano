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

#ifndef MPLAYERLOADER_H
#define MPLAYERLOADER_H

//For WId
#include <QtGui/QWidget>

#include <QtCore/QString>
#include <QtCore/QStringList>

class MediaSettings;
class MPlayerProcess;

class QObject;

/**
 * Helps to launch a MPlayer process.
 *
 * Acts like a simple proxy.
 * This class might become more "intelligent" in the future.
 *
 * @author Tanguy Krotoff
 */
class MPlayerLoader {
public:

	static MediaSettings settings;

	static MPlayerProcess * createNewMPlayerProcess(QObject * parent);

	/**
	 * Starts an empty MPlayerProcess in order to get MPlayer version number.
	 *
	 * Using the MPlayer version number, we performs some tests (see MPlayerLoader.cpp)
	 * and thus we need to know the MPlayer version number as soon as possible.
	 */
	static void startMPlayerVersion(QObject * parent);

	/**
	 * Loads a media without playing it.
	 *
	 * Only tries to detect meta-data informations for a media.
	 *
	 * @param process the MPlayerProcess to load
	 * @param filename media to load
	 */
	static void loadMedia(MPlayerProcess * process, const QString & filename);

	/**
	 * Starts/plays a media given its MPlayerProcess.
	 *
	 * Starts a new MPlayer instance.
	 *
	 * @param process the MPlayerProcess to start
	 * @param filename media to play
	 * @param videoWidgetId QWidget ID where the video (if any) will be played
	 */
	static void start(MPlayerProcess * process, const QString & filename, WId videoWidgetId);

	/**
	 * Restarts/replays a media given its MPlayerProcess.
	 *
	 * Restarts a the latest known MPlayer instance.
	 *
	 * This will use the previously known arguments and other infos.
	 *
	 * @param process the MPlayerProcess to start
	 * @param arguments MPlayerProcess arguments (options for MPlayer); if empty then previous arguments are used
	 * @param filename media to play; if empty then previous filename is used
	 * @param seek position inside the media in milliseconds; if -1 then previous media position is used
	 */
	static void restart(MPlayerProcess * process, const QStringList & arguments = QStringList(), const QString & filename = QString(), qint64 seek = -1);

private:

	MPlayerLoader();
	~MPlayerLoader();

	/**
	 * Reads the media settings to use them as arguments for the MPlayer process.
	 *
	 * @see http://www.mplayerhq.hu/DOCS/man/en/mplayer.1.html for a complete list of MPlayer parameters
	 * @return list of arguments for the MPlayer process
	 */
	static QStringList readMediaSettings();

	/** Code factorization. */
	static void start(MPlayerProcess * process, const QStringList & arguments, const QString & filename, WId videoWidgetId, qint64 seek);
};

#endif	//MPLAYERLOADER_H
