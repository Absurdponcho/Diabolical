#pragma once
#include "..\DiabolicalTypes.h"
#include <random>
#include <time.h>
namespace DRand
{
	static bool bAlwaysRandomizeSeed = true;
	class FastRandom32
	{
		FastRandom32();
		FastRandom32(types::UInt32 seed);

		types::UInt32 RandomUInt();
		types::UInt32 RandomUInt(types::UInt32 min, types::UInt32 max);

		types::Int32 RandomInt();
		types::Int32 RandomInt(types::Int32 min, types::Int32 max);

		types::Float32 RandomFloat();
		types::Float32 RandomFloat(types::Float32 min, types::Float32 max);

		void DiscardRandom(types::UInt32 nTimes);
		std::minstd_rand rand32;
	};

	class MersenneRandom64
	{
		MersenneRandom64();
		MersenneRandom64(types::UInt64 seed);

		types::UInt64 RandomUInt();
		types::UInt64 RandomUInt(types::UInt64 min, types::UInt64 max);

		types::Int64 RandomInt();
		types::Int64 RandomInt(types::Int64 min, types::Int64 max);

		types::Float64 RandomFloat();
		types::Float64 RandomFloat(types::Float64 min, types::Float64 max);

		void DiscardRandom(types::UInt64 nTimes);
		std::mt19937_64 rand64;
	};
};
