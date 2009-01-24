/*  This file is part of the KDE project
    Copyright (C) 2007 Matthias Kretz <kretz@kde.org>

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

#ifndef ABSTRACTMEDIASTREAM2_P_H
#define ABSTRACTMEDIASTREAM2_P_H

#include "phonon_export.h"
#include "abstractmediastream2.h"
#include "abstractmediastream_p.h"
#include "mediaobject_p.h"

#include "medianodedestructionhandler_p.h"
#include "streameventqueue_p.h"

QT_BEGIN_NAMESPACE

class MediaObjectPrivate;
class QAbstractEventDispatcher;

namespace Phonon
{
class PHONON_EXPORT AbstractMediaStream2Private : public AbstractMediaStreamPrivate, private LockFreeQueueBase::DataReadyHandler
{
    friend class MediaObject;
    friend class MediaSourcePrivate;
    Q_DECLARE_PUBLIC(AbstractMediaStream2)
    protected:
        AbstractMediaStream2Private();
        ~AbstractMediaStream2Private();

        virtual void setStreamSize(qint64 newSize);
        virtual void setStreamSeekable(bool s);
        virtual void writeData(const QByteArray &data);
        virtual void endOfData();
        virtual void dataReady();
        void _k_handleStreamEvent();

        StreamEventQueue *streamEventQueue;
        QAbstractEventDispatcher *eventDispatcher;
        int connectReset;
        bool firstConnect;
};
} // namespace Phonon

QT_END_NAMESPACE

#endif // ABSTRACTMEDIASTREAM2_P_H
// vim: sw=4 sts=4 et tw=100
