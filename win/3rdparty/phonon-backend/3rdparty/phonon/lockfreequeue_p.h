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

#ifndef LOCKFREEQUEUE_P_H
#define LOCKFREEQUEUE_P_H

#include <QtCore/QVector>

class LockFreeQueueBasePrivate;
struct MemoryPool;
class LockFreeQueueBase
{
    friend class LockFreeQueueBasePrivate;
    public:
        struct DataReadyHandler
        {
            virtual ~DataReadyHandler() {}
            virtual void dataReady() = 0;
        };

        void setDataReadyHandler(DataReadyHandler *);

        int size() const;
        bool isEmpty() const { return 0 == size(); }

    protected:
        friend struct MemoryPool;
        LockFreeQueueBase();
        ~LockFreeQueueBase();

    public:
        class NodeBase;
        typedef volatile NodeBase * NodeBasePointer;
        class NodeBase
        {
            public:
                inline NodeBase(int p) : priority(p) {}
                inline NodeBase(NodeBase *n) : next(n), priority(0) {}
                NodeBasePointer next;
                int priority;

                inline void ref() { refCount.ref(); }
                inline void deref() { if (!refCount.deref()) delete this; }

            protected:
                ~NodeBase() { Q_ASSERT(refCount == 0); }
            private:
                QAtomicInt refCount;
        };

        struct NodeBaseKeepNodePool : public NodeBase
        {
            inline NodeBaseKeepNodePool(int priority) : NodeBase(priority) {}
            // allocation is a bottleneck in _enqueue
            void *operator new(size_t s);
            void operator delete(void *p, size_t s);

            static void clear();
            static void setPoolSize(int);
            static int poolSize();
        };

        typedef NodeBase             StdNewDeleteMemoryManagement;
        typedef NodeBaseKeepNodePool KeepNodePoolMemoryManagement;
    protected:

        void _enqueue(NodeBase *);
        NodeBase *_acquireHeadNode();
        NodeBase *_acquireHeadNodeBlocking();

        LockFreeQueueBasePrivate *const d;

};

template<class T, class MemoryManagementNodeBase = LockFreeQueueBase::StdNewDeleteMemoryManagement>
class LockFreeQueue : public LockFreeQueueBase
{
    public:
        enum BlockingSwitch {
            BlockUnlessEmpty,
            NeverBlock
        };

        struct Node : public MemoryManagementNodeBase
        {
            inline Node(const T &d, int priority) : MemoryManagementNodeBase(priority), data(d) {}
            T data;
        };

        inline void enqueue(const T &data, int priority = 0)
        {
            _enqueue(new Node(data, 0));
        }

        inline void dequeue(QVector<T> &data, BlockingSwitch block = BlockUnlessEmpty)
        {
            int count = 0;
            while (count < data.capacity()) {
                NodeBase *node = (block == NeverBlock) ? _acquireHeadNode() : _acquireHeadNodeBlocking();
                if (!node) {
                    break;
                }
                if (count < data.size()) {
                    data[count] = static_cast<Node *>(node)->data;
                } else {
                    data << static_cast<Node *>(node)->data;
                }
                ++count;
                node->deref();
            }
            data.resize(count);
        }

        inline bool dequeue(T *data, BlockingSwitch block = BlockUnlessEmpty)
        {
            NodeBase *node = (block == NeverBlock) ? _acquireHeadNode() : _acquireHeadNodeBlocking();
            if (node) {
                *data = static_cast<Node *>(node)->data;
                node->deref();
                return true;
            }
            return false;
        }

        inline LockFreeQueue<T> &operator<<(const T &data) { enqueue(data); return *this; }
        inline LockFreeQueue<T> &operator>>(T &data)
        {
            NodeBase *node = _acquireHeadNodeBlocking();
            if (node) {
                data = static_cast<Node *>(node)->data;
                node->deref();
            }
            return *this;
        }
};

#endif // LOCKFREEQUEUE_P_H
