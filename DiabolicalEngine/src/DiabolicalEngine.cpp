#include "DiabolicalEngine.h"


flecs::world ecs;

void DEngine::Init(int argc, char* argv[])
{

}

void DEngine::Run()
{
	while (ecs.progress()) {}
}