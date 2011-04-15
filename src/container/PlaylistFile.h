/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2001 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef TANO_PLAYLISTFILE_H_
#define TANO_PLAYLISTFILE_H_

#include <QtCore/QString>

class PlaylistFile
{
public:
	PlaylistFile(const QString &country = 0);
	~PlaylistFile();

	QString country() const { return _country; }
	void setCountry(const QString &s) { _country = s; }
	QString path() const { return _path; }
	void setPath(const QString &s) { _path = s; }
	QString title() const { return _title; }
	void setTitle(const QString &s) { _title = s; }

private:
	QString _country;
	QString _path;
	QString _title;
};

#endif // TANO_PLAYLISTFILE_H_
