#include <Windows.h>
#include <cstdint>
#include <vector>
#include <zlib.h>

namespace Archive
{
	struct ArchiveHeader
	{
		char Magic[4];
		int64_t VersionID;
		int64_t SizeOfHeader;
		int64_t NumberOfFiles;
	};

	struct FileDescriptor
	{
		char Name[64];
		int64_t Offset;
		int64_t SizeInBytes;
		bool bZlibCompressed;
	};

	struct Archive
	{
		ArchiveHeader Header;
		FileDescriptor Files[];
	};
}


