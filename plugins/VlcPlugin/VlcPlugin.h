/****************************************************************************
* VlcPlugin.h: Recorder Plugin using VLC
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

#ifndef TANO_VLCPLUGIN_H_
#define TANO_VLCPLUGIN_H_

#include <QtCore/QString>

#include "plugins/RecorderPlugins.h"

class VlcPlugin : public RecorderPlugin
{
public:
	VlcPlugin();
	~VlcPlugin();

	void record(const QString &channelName,
				const QString &channelUrl,
				const QString &recordingDir);
	void stop();
	bool isRecording() const;
	bool isValid() const {return true;};
	QString output() const {return _output;};

private:
	QString _slash;
	QString _output;
};

#endif // TANO_VLCPLUGIN_H_
