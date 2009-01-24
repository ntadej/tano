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

#include "MPlayerVersion.h"

//For MPLAYER_1_0_RC2_SVN, MPLAYER_1_0_RC1_SVN and other defines
#include "MPlayerProcess.h"

#include <QtCore/QRegExp>
#include <QtCore/QDebug>

int MPlayerVersion::parse(const QString & line) {
	//Examples:
	//From older to most recent
	//
	//MPlayer 1.0pre4-3.3.3 (C) 2000-2004 MPlayer Team
	//
	//Windows, official 1.0rc2 binary:
	//MPlayer 1.0rc2-4.2.1 (C) 2000-2007 MPlayer Team
	//MPlayer 1.0rc2-4.2.3 (C) 2000-2007 MPlayer Team
	//
	//Ubuntu 8.10:
	//MPlayer 1.0rc2-4.3.2 (C) 2000-2007 MPlayer Team
	//
	//Windows, rvm build:
	//MPlayer dev-SVN-r27130-3.4.5 (with -volume) (C) 2000-2008 MPlayer Team
	//
	//Ubuntu 8.10, rvm build:
	//MPlayer dev-SVN-r27900-4.3.2 (C) 2000-2008 MPlayer Team
	//
	//Windows, http://sourceforge.net/projects/mplayer-win32/ build:
	//MPlayer Sherpya-SVN-r28126-4.2.5 (C) 2000-2008 MPlayer Team
	//
	//openSUSE 11.1:
	//MPlayer dev-SVN-r27637-4.3-openSUSE Linux 11.1 (i686)-Packman (C) 2000-2008 MPlayerTeam
	//
	static QRegExp rx_mplayer_revision("^MPlayer (.*)-r(\\d+)(.*)");
	static QRegExp rx_mplayer_version("^MPlayer ([a-z,0-9,.]+)-(.*)");

	QString version(line);
	int revision = MPlayerProcess::MPLAYER_VERSION_FAILED;

	if (rx_mplayer_revision.indexIn(version) > -1) {
		revision = rx_mplayer_revision.cap(2).toInt();
		qDebug() << __FUNCTION__ << "MPlayer SVN revision:" << revision;
	}

	else if (rx_mplayer_version.indexIn(version) > -1) {
		version = rx_mplayer_version.cap(1);
		qDebug() << __FUNCTION__ << "MPlayer version:" << version;
		if (version == "1.0rc2") {
			revision = MPlayerProcess::MPLAYER_1_0_RC2_SVN;
		}
		else if (version == "1.0rc1") {
			revision = MPlayerProcess::MPLAYER_1_0_RC1_SVN;
		} else {
			qCritical() << "Error: unknown MPlayer version";
		}
	}

	if (revision == MPlayerProcess::MPLAYER_VERSION_FAILED) {
		qCritical() << "Error: couldn't parse MPlayer revision:" << version;
	}

	return revision;
}
