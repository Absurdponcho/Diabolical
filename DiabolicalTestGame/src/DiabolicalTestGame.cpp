#include "DiabolicalEngine.h"

int main (int argc, char *argv[])
{
	DEngine::Init(argc, argv);

	for (auto& Tuple : DCommandLine::Get().GetKeyValuePairs())
	{
		std::cout << std::get<0>(Tuple) << " : " << std::get<1>(Tuple) << std::endl;
	}

	DEngine::Run();
}