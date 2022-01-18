#include "Check.h"
#include "Logging/Logging.h"
#include "DiabolicalEngine.h"

void CheckCrash(const char* Func, const char* Reason, int Line)
{
	Logging::LogPlain(DString::Format("CHECK FAILED! %s (%i): %s", Reason, Line, Func), 12);
	DEngine::AtExit();
	abort();
}

