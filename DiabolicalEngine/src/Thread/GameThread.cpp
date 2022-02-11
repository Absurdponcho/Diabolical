#include "GameThread.h"

DVector<DActionBase*> DGameThread::LaterFuncs;
DMutexHandle DGameThread::Mutex;
