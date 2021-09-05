#include <iostream>
#include "Archive.h"
#include "ArchiveTool.h"
int main(int argc, char* argv[])
{
    std::cout << "ArchiveTool v0.01\n";
    if (argc != 2) {
        std::cout << "Error: Not Enough Arguments\nUsage:ArchiveTool [directory]\n";
        exit(-1);
    }
    std::filesystem::path directory = argv[1];
    if (!std::filesystem::exists(directory)) {
        std::cout << "Error: Bad Path \nUsage:ArchiveTool [directory]\n";
        exit(-2);
    }
}
