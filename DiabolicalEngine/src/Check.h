#pragma once
#include <assert.h>
#include "Meta/Meta.h"

void CheckCrash(const char* Func, const char* Reason, int Line) ;

#define Check(x) if (!(x)){CheckCrash(#x, __FUNCTION__, __LINE__);}; 



