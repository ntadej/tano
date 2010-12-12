/****************************************************************************
* FripTv.h: Recorder Plugin using friptv
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

#ifndef TANOPLUGIN_FRIPTV_H_
#define TANOPLUGIN_FRIPTV_H_

#include <QtCore/QProcess>
#include <QtCore/QString>

#include "RecorderPlugins.h"

class FripTv : public RecorderPlugin
{
public:
	FripTv();
	~FripTv();

	void record(const QString &channelName,
				const QString &channelUrl,
				const QString &recordingDir);
	void stop();
	bool isRecording() const;
	bool isValid() const { return fripExists(); }
	QString output() const { return _output; }

private:
	bool fripExists() const;
	QString fripPath() const;

	QProcess *_fripProcess;
	QString _fripPath;

	QString _slash;
	QString _output;
};

#endif // TANOPLUGIN_FRIPTV_H_
