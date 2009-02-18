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

#ifndef PHONON_STREAMEVENTQUEUE_P_H
#define PHONON_STREAMEVENTQUEUE_P_H

#include <QtCore/QVariant>
#include "lockfreequeue_p.h"

QT_BEGIN_NAMESPACE

namespace Phonon
{

class StreamEventQueue
{
    public:
        StreamEventQueue();
        ~StreamEventQueue();

        void setBackendCommandHandler(LockFreeQueueBase::DataReadyHandler *);
        void setFrontendCommandHandler(LockFreeQueueBase::DataReadyHandler *);

        enum CommandType {
            // Frontend -> Backend
            SetStreamSize,
            SetSeekable,
            Write,
            EndOfData,
            SeekDone,
            ResetDone,
            ConnectDone,

            // Backend -> Frontend
            NeedData,
            EnoughData,
            Seek,
            Reset,
            Connect
        };

        struct Command
        {
            inline Command(CommandType &c, const QVariant &d) : data(d), command(c) {}
            inline Command() {}
            QVariant data;
            CommandType command;
        };

        // called from AbstractMediaStream thread
        void sendToBackend(CommandType, const QVariant & = QVariant());
        bool nextCommandForFrontend(Command *);

        // called from StreamInterface thread
        void sendToFrontend(CommandType, const QVariant & = QVariant());
        bool nextCommandForBackend(Command *);

        void ref() { m_references.ref(); }
        void deref() { if (!m_references.deref()) delete this; }

    private:
        LockFreeQueue<Command, LockFreeQueueBase::KeepNodePoolMemoryManagement> m_forBackend;
        LockFreeQueue<Command, LockFreeQueueBase::KeepNodePoolMemoryManagement> m_forFrontend;
        int m_dropWriteCommands;
        int m_connecting;
        QAtomicInt m_references;
};

} // namespace Phonon

QT_END_NAMESPACE

#endif // PHONON_STREAMEVENTQUEUE_P_H
