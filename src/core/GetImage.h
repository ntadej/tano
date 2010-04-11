/****************************************************************************
* GetImage.h: Image downloader class
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

#ifndef TANO_GETIMAGE_H_
#define TANO_GETIMAGE_H_

#include <QtCore/QFile>
#include <QtNetwork/QHttp>

class GetImage : public QHttp
{
Q_OBJECT
public:
	GetImage(QObject *parent = 0);
	~GetImage();

	void getImage(const QString &u);

signals:
	void image(const QString);

private slots:
	void httpRequestFinished(const int &requestId, const bool &error);

private:
	QFile *_file;
	int _httpGetId;
};

#endif // TANO_GETIMAGE_H_
