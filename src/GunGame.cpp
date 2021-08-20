#include <iostream>
#include "Logging/Logging.h"

int main()
{
    Logging::Log("main", "Main function lel");
    Logging::LogWarning("main", "Main warning lel");
    Logging::LogError("main", "Main error lel");
}
