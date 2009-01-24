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

#ifndef MPLAYERVERSION_H
#define MPLAYERVERSION_H

#include <QtCore/QString>

/**
 * Gets MPlayer SVN revision number.
 *
 * This code could be moved inside MPlayerProcess
 *
 * @author Tanguy Krotoff
 */
class MPlayerVersion {
public:

	/**
	 * Parses the MPlayer SVN revision number from the line and returns it.
	 *
	 * @param line line to parse to get MPlayer SVN revision number
	 * @return MPlayer SVN revision number; 0 if parsing failed
	 */
	static int parse(const QString & line);

private:

	MPlayerVersion();
	~MPlayerVersion();
};

#endif	//MPLAYERVERSION_H
