#include <iostream>
#include "Logging/Logging.h"
#include "WindowManager.h"
#undef main

int main(int argc, char** argv)
{
    Logging::Log("main", "Main function lel");
    Logging::LogWarning("main", "Main warning lel");
    Logging::LogError("main", "Main error lel");
    WindowManager WManager = WindowManager(
        "GunGame",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, 0);

    while (1);
    return 0;
}
