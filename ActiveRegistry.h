#ifndef ACTIVEREGISTRY_H
#define ACTIVEREGISTRY_H

#include <jni.h>
#include <QMutex>
#include <QMutexLocker>
#include <QSet>

class ActiveRegistry
{
public:
    static void registerInstance(void* pointer)
    {
        QMutexLocker locker(&mutex());
        instances().insert(pointer);
    }

    static void unregisterInstance(void* pointer)
    {
        QMutexLocker locker(&mutex());
        instances().remove(pointer);
    }

    static bool contains(jlong nativePointer)
    {
        QMutexLocker locker(&mutex());
        return instances().contains(reinterpret_cast<void*>(nativePointer));
    }

private:
    static QMutex& mutex()
    {
        static QMutex value;
        return value;
    }

    static QSet<void*>& instances()
    {
        static QSet<void*> value;
        return value;
    }
};

#endif // ACTIVEREGISTRY_H
