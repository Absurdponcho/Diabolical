#include "GameThread.h"

std::vector<DActionBase*> DGameThread::LaterFuncs;
DMutexHandle DGameThread::Mutex;
