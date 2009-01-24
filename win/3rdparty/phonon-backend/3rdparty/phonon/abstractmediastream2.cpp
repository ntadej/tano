/*  This file is part of the KDE project
    Copyright (C) 2007-2008 Matthias Kretz <kretz@kde.org>

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

#include "abstractmediastream2.h"
#include "abstractmediastream2_p.h"
#include "mediaobjectinterface.h"
#include "mediaobject_p.h"
#include "phononnamespace_p.h"

#include <QtCore/QAbstractEventDispatcher>
#include <QtCore/QEvent>

QT_BEGIN_NAMESPACE

namespace Phonon
{

bool AbstractMediaStream2::event(QEvent *e)
{
    if (e->type() == QEvent::ThreadChange) {
        Q_D(AbstractMediaStream2);
        if (d->eventDispatcher) {
            pWarning() << "Do not call moveToThread on an AbstractMediaStream2 object when it is in use.";
            QObject::disconnect(d->eventDispatcher, SIGNAL(awake()), this, SLOT(_k_handleStreamEvent()));
            d->eventDispatcher = 0;
        }
    }
    return AbstractMediaStream::event(e);
}

void AbstractMediaStream2Private::dataReady()
{
    Q_Q(AbstractMediaStream2);
    if (!eventDispatcher) {
        eventDispatcher = QAbstractEventDispatcher::instance(q->thread());
        if (!eventDispatcher) {
            pError() << "AbstractMediaStream2 needs to run in a thread with QEventLoop";
            return;
        } else {
            QObject::connect(eventDispatcher, SIGNAL(awake()), q, SLOT(_k_handleStreamEvent()), Qt::DirectConnection);
        }
    }
    eventDispatcher->wakeUp();
}

void AbstractMediaStream2Private::_k_handleStreamEvent()
{
    Q_ASSERT(streamEventQueue);
    Q_Q(AbstractMediaStream2);
    StreamEventQueue::Command c;
    while (streamEventQueue->nextCommandForFrontend(&c)) {
        switch (c.command) {
        case StreamEventQueue::SetStreamSize:
        case StreamEventQueue::SetSeekable:
        case StreamEventQueue::Write:
        case StreamEventQueue::EndOfData:
        case StreamEventQueue::SeekDone:
        case StreamEventQueue::ResetDone:
        case StreamEventQueue::ConnectDone:
            pFatal("AbstractMediaStream2 received wrong Command");
            break;
        case StreamEventQueue::NeedData:
            q->needData(qvariant_cast<quint32>(c.data));
            break;
        case StreamEventQueue::EnoughData:
            q->enoughData();
            break;
        case StreamEventQueue::Seek:
            q->seekStream(qvariant_cast<quint64>(c.data));
            break;
        case StreamEventQueue::Reset:
            q->reset();
            break;
        case StreamEventQueue::Connect:
            if (firstConnect) {
                firstConnect = false;
                streamEventQueue->sendToBackend(StreamEventQueue::ConnectDone);
                streamEventQueue->sendToBackend(StreamEventQueue::SetSeekable, streamSeekable);
                if (streamSize != 0) {
                    streamEventQueue->sendToBackend(StreamEventQueue::SetStreamSize, streamSize);
                }
            } else {
                ++connectReset;
                q->reset();
            }
            break;
        }
    }
}

AbstractMediaStream2::AbstractMediaStream2(QObject *parent)
    : AbstractMediaStream(*new AbstractMediaStream2Private, parent)
{
}

AbstractMediaStream2::AbstractMediaStream2(AbstractMediaStream2Private &dd, QObject *parent)
    : AbstractMediaStream(dd, parent)
{
}

void AbstractMediaStream2Private::setStreamSize(qint64 newSize)
{
    streamSize = newSize;
    streamEventQueue->sendToBackend(StreamEventQueue::SetStreamSize, newSize);
}

void AbstractMediaStream2Private::setStreamSeekable(bool s)
{
    streamSeekable = s;
    streamEventQueue->sendToBackend(StreamEventQueue::SetSeekable, s);
}

void AbstractMediaStream2Private::writeData(const QByteArray &data)
{
    streamEventQueue->sendToBackend(StreamEventQueue::Write, data);
}

void AbstractMediaStream2Private::endOfData()
{
    streamEventQueue->sendToBackend(StreamEventQueue::EndOfData);
}

void AbstractMediaStream2::resetDone()
{
    Q_D(AbstractMediaStream2);
    if (d->connectReset > 0) {
        --d->connectReset;
        d->streamEventQueue->sendToBackend(StreamEventQueue::ConnectDone);
    } else {
        d->streamEventQueue->sendToBackend(StreamEventQueue::ResetDone);
    }
    d->streamEventQueue->sendToBackend(StreamEventQueue::SetSeekable, d->streamSeekable);
    if (d->streamSize != 0) {
        d->streamEventQueue->sendToBackend(StreamEventQueue::SetStreamSize, d->streamSize);
    }
}

void AbstractMediaStream2::seekStreamDone()
{
    d_func()->streamEventQueue->sendToBackend(StreamEventQueue::SeekDone);
}

AbstractMediaStream2Private::AbstractMediaStream2Private()
    : streamEventQueue(new StreamEventQueue),
    eventDispatcher(0),
    connectReset(0),
    firstConnect(true)
{
    streamEventQueue->ref();
    streamEventQueue->setFrontendCommandHandler(this);
}

AbstractMediaStream2Private::~AbstractMediaStream2Private()
{
    // setFrontendCommandHandler blocks internally until a handler that might be called right now is
    // done. That means AbstractMediaStream2Private::dataReady will either not run again or it will
    // be done before this object and our q object are deleted.
    streamEventQueue->setFrontendCommandHandler(0);
    streamEventQueue->deref();
    streamEventQueue = 0;
    // if eventDispatcher->wakeUp() was called and we are now still connected to its awake() signal
    // then the QObject will be deleted until execution reaches the event loop again and
    // _k_handleStreamEvent will not get called
}

} // namespace Phonon

QT_END_NAMESPACE

#include "moc_abstractmediastream2.cpp"

// vim: sw=4 sts=4 et tw=100
