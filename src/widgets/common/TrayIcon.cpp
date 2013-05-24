/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

#include "core/Common.h"

#include "TrayIcon.h"

TrayIcon::TrayIcon(QMenu *menu,
                   QObject *parent)
    : QSystemTrayIcon(parent),
      _currentlyPlaying(""),
      _currentlyRecording("")
{
    setContextMenu(menu);
    setIcon(QIcon(":/logo/48x48/logo.png"));
    setToolTip(Tano::name());

    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

TrayIcon::~TrayIcon() { }

void TrayIcon::iconActivated(const QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        emit restoreClick();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }
}

void TrayIcon::message(const Tano::Id &type,
                       const QStringList &arg)
{
    if (arg.isEmpty())
        return;

    if (type == Tano::Record) {
        if (arg.size() == 0) {
            showMessage(tr("Recorder"), tr("Recording stopped."), QSystemTrayIcon::Information, 10000);
        } else if (arg.size() == 2) {
            showMessage(tr("Recorder"), tr("Recording %1 to %2 \nStop manually!").arg(arg[0], arg[1]), QSystemTrayIcon::Information, 10000);
        } else if (arg.size() == 3) {
            showMessage(tr("Recorder"), tr("Recording %1 to %2 \nEnd time: %3").arg(arg[0], arg[1], arg[2]), QSystemTrayIcon::Information, 10000);
        }
    }
}

void TrayIcon::changeToolTip(const Tano::Id &type,
                             const QString &text)
{
    if (type == Tano::Main) {
        if (!text.isEmpty()) {
            _currentlyPlaying = tr("Playing:") + " " + text;
        } else {
            _currentlyPlaying = text;
        }
    } else if (type == Tano::Record) {
        if (!text.isEmpty()) {
            _currentlyRecording = tr("Recording:") + " " + text;
        } else {
            _currentlyPlaying = text;
        }
    }

    if (_currentlyPlaying.isEmpty() && _currentlyRecording.isEmpty())
        setToolTip(Tano::name());
    else if (!_currentlyPlaying.isEmpty() && _currentlyRecording.isEmpty())
        setToolTip(Tano::name() + "\n" + _currentlyPlaying);
    else if (_currentlyPlaying.isEmpty() && !_currentlyRecording.isEmpty())
        setToolTip(Tano::name() + "\n" + _currentlyRecording);
    else
        setToolTip(Tano::name() + "\n" + _currentlyPlaying + "\n" + _currentlyRecording);
}
