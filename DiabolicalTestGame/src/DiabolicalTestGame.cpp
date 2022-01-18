#include "DiabolicalEngine.h"

int main (int argc, char *argv[])
{
	DEngine::Init(argc, argv);

	if (const DString* Value = DCommandLine::Get().GetValue("123"))
	{
		std::cout << *Value;
	}

	DEngine::Run();
}