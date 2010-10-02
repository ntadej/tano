/****************************************************************************
* GetFile.h: File downloader class
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
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
	void image(const QString);

private slots:
	void httpRequestFinished(const int &requestId,
							 const bool &error);

private:
	QFile *_file;
	int _httpGetId;
};

#endif // TANO_GETFILE_H_
