#pragma once
#include "ECS/flecs.h"
#include <iostream>
#include <string>

class DEngine
{
public:
	static void Init(int argc, char *argv[]);
	static void Run();

private:
	static class DGameManager* GameManager;
};