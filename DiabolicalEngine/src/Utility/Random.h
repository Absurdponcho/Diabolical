#pragma once
#include "..\DiabolicalTypes.h"
#include <random>
#include <time.h>
static bool bAlwaysRandomizeSeed = true;
class FastRandom32
{
	FastRandom32();
	FastRandom32(UInt32 seed);

	UInt32 RandomUInt();
	UInt32 RandomUInt(UInt32 min, UInt32 max);

	Int32 RandomInt();
	Int32 RandomInt(Int32 min, Int32 max);

	Float32 RandomFloat();
	Float32 RandomFloat(Float32 min, Float32 max);

	void DiscardRandom(UInt32 nTimes);
	std::minstd_rand rand32;
};

class MersenneRandom64
{
	MersenneRandom64();
	MersenneRandom64(UInt64 seed);

	UInt64 RandomUInt();
	UInt64 RandomUInt(UInt64 min, UInt64 max);

	Int64 RandomInt();
	Int64 RandomInt(Int64 min, Int64 max);

	Float64 RandomFloat();
	Float64 RandomFloat(Float64 min, Float64 max);

	void DiscardRandom(UInt64 nTimes);
	std::mt19937_64 rand64;
};

