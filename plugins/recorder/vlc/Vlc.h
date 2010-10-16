/****************************************************************************
* Vlc.h: Recorder Plugin using VLC
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

#ifndef TANOPLUGIN_VLC_H_
#define TANOPLUGIN_VLC_H_

#include <QtCore/QProcess>
#include <QtCore/QString>

#include "plugins/RecorderPlugins.h"

class Vlc : public RecorderPlugin
{
public:
	Vlc();
	~Vlc();

	void record(const QString &channelName,
				const QString &channelUrl,
				const QString &recordingDir);
	void stop();
	bool isRecording() const;
	bool isValid() const { return true; }
	QString output() const { return _output; }

private:
	QString _slash;
	QString _output;

	QProcess *_vlcProcess;
};

#endif // TANOPLUGIN_VLC_H_
