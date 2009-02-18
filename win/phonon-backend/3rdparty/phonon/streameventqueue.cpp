/*  This file is part of the KDE project
    Copyright (C) 2008 Matthias Kretz <kretz@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), Trolltech ASA 
    (or its successors, if any) and the KDE Free Qt Foundation, which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public 
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "streameventqueue_p.h"
#include "phononnamespace_p.h"
#include <QtCore/QtDebug>

namespace Phonon
{

//////////////////////////////////////////
// any thread

StreamEventQueue::StreamEventQueue()
    : m_dropWriteCommands(0),
    m_connecting(0)
{
}

StreamEventQueue::~StreamEventQueue()
{
}

void StreamEventQueue::setBackendCommandHandler(LockFreeQueueBase::DataReadyHandler *h)
{
    m_forBackend.setDataReadyHandler(h);
}

void StreamEventQueue::setFrontendCommandHandler(LockFreeQueueBase::DataReadyHandler *h)
{
    m_forFrontend.setDataReadyHandler(h);
}


//////////////////////////////////////////
// frontend thread

void StreamEventQueue::sendToBackend(CommandType command, const QVariant &data)
{
    m_forBackend.enqueue(Command(command, data));
}

bool StreamEventQueue::nextCommandForFrontend(Command *command)
{
    Q_ASSERT(command);
    return m_forFrontend.dequeue(command);
}


//////////////////////////////////////////
// backend thread

void StreamEventQueue::sendToFrontend(CommandType command, const QVariant &data)
{
    switch (command) {
    case Seek:
    case Reset:
        ++m_dropWriteCommands;
        break;
    case Connect:
        ++m_connecting;
        m_dropWriteCommands = 0;
        break;
    default:
        break;
    }
    m_forFrontend.enqueue(Command(command, data));
}

bool StreamEventQueue::nextCommandForBackend(Command *command)
{
    Q_ASSERT(command);
    bool valid = m_forBackend.dequeue(command);
    while (valid) {
        if (m_connecting) {
            if (command->command == ConnectDone) {
                --m_connecting;
            }
            // drop all other commands until m_connecting == 0
        } else if (m_dropWriteCommands) {
            switch (command->command) {
            case Write:
            case EndOfData:
                // drop until m_dropWriteCommands == 0
                break;
            case SeekDone:
            case ResetDone:
                --m_dropWriteCommands;
                break;
            case ConnectDone:
                pFatal("received a ConnectDone, but there was no Connect request");
                break;
            default:
                return valid;
            }
        } else {
            return valid;
        }
        valid = m_forBackend.dequeue(command);
    }
    return valid;
}

} // namespace Phonon
