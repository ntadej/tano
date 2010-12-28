/****************************************************************************
* FripTV - Tano Recorder Plugin
* Copyright (C) 2010 Tadej Novak <tadej@tano.si>
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
				const QString &file);
	void stop();
	bool isRecording() const;
	bool isValid() const { return fripExists(); }

private:
	bool fripExists() const;
	QString fripPath() const;

	QProcess *_fripProcess;
	QString _fripPath;
};

#endif // TANOPLUGIN_FRIPTV_H_
