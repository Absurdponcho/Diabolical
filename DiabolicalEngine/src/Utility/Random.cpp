#include "Random.h"

DRand::FastRandom32::FastRandom32()
{
    UInt32 seed = 1;

    if (DRand::bAlwaysRandomizeSeed) {
        srand(_time32(nullptr));
        seed = rand32() ^ 0xC;
    }

    rand32.seed(seed);
}

DRand::FastRandom32::FastRandom32(UInt32 seed)
{
    rand32.seed(seed);
}

UInt32 DRand::FastRandom32::RandomUInt()
{
    return (UInt32) rand32();
}

UInt32 DRand::FastRandom32::RandomUInt(UInt32 min, UInt32 max)
{
    return (UInt32) rand32() % (max - min) + min;
}

Int32 DRand::FastRandom32::RandomInt()
{
    return (Int32) rand32();
}

Int32 DRand::FastRandom32::RandomInt(Int32 min, Int32 max)
{
    return (Int32) rand32() % (max - min) + min;
}

Float32 DRand::FastRandom32::RandomFloat()
{
    return RandomFloat(0, 1);
}

Float32 DRand::FastRandom32::RandomFloat(Float32 min, Float32 max)
{
    return min + ((max - min) * ((Float32) rand32() / (Float32)RAND_MAX));
}

void DRand::FastRandom32::DiscardRandom(UInt32 nTimes)
{
    int q = 0;
    for (auto i = 0u; i < nTimes; i++)
        q = rand32();
}

DRand::MersenneRandom64::MersenneRandom64()
{
    UInt64 seed = rand64.default_seed;

    if (DRand::bAlwaysRandomizeSeed) {
        srand(_time32(nullptr));
        seed = rand64() ^ 0xC;
    }

    rand64.seed(seed);
}

DRand::MersenneRandom64::MersenneRandom64(UInt64 seed)
{
    rand64.seed(seed);
}

UInt64 DRand::MersenneRandom64::RandomUInt()
{
    return (UInt64) rand64();
}

UInt64 DRand::MersenneRandom64::RandomUInt(UInt64 min, UInt64 max)
{
    return (UInt64) rand64() % (max - min) + min;
}

Int64 DRand::MersenneRandom64::RandomInt()
{
    return (Int64) rand64();
}

Int64 DRand::MersenneRandom64::RandomInt(Int64 min, Int64 max)
{
    return (Int64) rand64() % (max - min) + min;
}

Float64 DRand::MersenneRandom64::RandomFloat()
{
    return RandomFloat(0, 1);
}

Float64 DRand::MersenneRandom64::RandomFloat(Float64 min, Float64 max)
{
    return min + ((max - min) * ((Float64)rand64() / (Float64)RAND_MAX));
}

void DRand::MersenneRandom64::DiscardRandom(UInt64 nTimes)
{
    {
        int q = 0;
        for (auto i = 0u; i < nTimes; i++)
            q = rand64();
    }
}
