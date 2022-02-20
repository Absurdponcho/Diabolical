#include "Check.h"
#include "Logging/Logging.h"
#include "DiabolicalEngine.h"

void CheckCrash(const char* Func, const char* Reason, int Line)
{
	DString ErrorMessage = DString::Format("CHECK FAILED! %s (%i): %s\n", Reason, Line, Func);
	Logging::LogPlain(ErrorMessage, 12);
	DEngine::AtExit();
	abort();
}

