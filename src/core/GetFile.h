/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_GETFILE_H_
#define TANO_GETFILE_H_

#include <QtCore/QFile>
#include <QtNetwork/QHttp>

class GetFile : public QHttp
{
Q_OBJECT
public:
	GetFile(QObject *parent = 0);
	~GetFile();

	void getFile(const QString &fileUrl,
				 const QString &location = 0);

signals:
	void file(const QString &);

private slots:
	void httpRequestFinished(const int &requestId,
							 const bool &error);

private:
	QFile *_file;
	int _httpGetId;
};

#endif // TANO_GETFILE_H_
