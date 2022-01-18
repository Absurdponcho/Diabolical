#include "DiabolicalEngine.h"

int main (int argc, char *argv[])
{
	DEngine::Init(argc, argv);

	LOG("Test1");
	LOG("Test2");
	if (const DString* Value = DCommandLine::Get().GetValue("123"))
	{
		std::cout << *Value;
	}
	LOG_ERR("Test Errror");
	LOG_WARN("Test Warn");
	Check(false);
	DEngine::Run();
}