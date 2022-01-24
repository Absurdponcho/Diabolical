#include "Random.h"

DRand::FastRandom32::FastRandom32()
{
    types::UInt32 seed = 1;

    if (DRand::bAlwaysRandomizeSeed) {
        srand(_time32(nullptr));
        seed = rand32() ^ 0xC;
    }

    rand32.seed(seed);
}

DRand::FastRandom32::FastRandom32(types::UInt32 seed)
{
    rand32.seed(seed);
}

types::UInt32 DRand::FastRandom32::RandomUInt()
{
    return (types::UInt32) rand32();
}

types::UInt32 DRand::FastRandom32::RandomUInt(types::UInt32 min, types::UInt32 max)
{
    return (types::UInt32) rand32() % (max - min) + min;
}

types::Int32 DRand::FastRandom32::RandomInt()
{
    return (types::Int32) rand32();
}

types::Int32 DRand::FastRandom32::RandomInt(types::Int32 min, types::Int32 max)
{
    return (types::Int32) rand32() % (max - min) + min;
}

types::Float32 DRand::FastRandom32::RandomFloat()
{
    return RandomFloat(0, 1);
}

types::Float32 DRand::FastRandom32::RandomFloat(types::Float32 min, types::Float32 max)
{
    return min + ((max - min) * ((types::Float32) rand32() / (types::Float32)RAND_MAX));
}

void DRand::FastRandom32::DiscardRandom(types::UInt32 nTimes)
{
    int q = 0;
    for (auto i = 0u; i < nTimes; i++)
        q = rand32();
}

DRand::MersenneRandom64::MersenneRandom64()
{
    types::UInt64 seed = rand64.default_seed;

    if (DRand::bAlwaysRandomizeSeed) {
        srand(_time32(nullptr));
        seed = rand64() ^ 0xC;
    }

    rand64.seed(seed);
}

DRand::MersenneRandom64::MersenneRandom64(types::UInt64 seed)
{
    rand64.seed(seed);
}

types::UInt64 DRand::MersenneRandom64::RandomUInt()
{
    return (types::UInt64) rand64();
}

types::UInt64 DRand::MersenneRandom64::RandomUInt(types::UInt64 min, types::UInt64 max)
{
    return (types::UInt64) rand64() % (max - min) + min;
}

types::Int64 DRand::MersenneRandom64::RandomInt()
{
    return (types::Int64) rand64();
}

types::Int64 DRand::MersenneRandom64::RandomInt(types::Int64 min, types::Int64 max)
{
    return (types::Int64) rand64() % (max - min) + min;
}

types::Float64 DRand::MersenneRandom64::RandomFloat()
{
    return RandomFloat(0, 1);
}

types::Float64 DRand::MersenneRandom64::RandomFloat(types::Float64 min, types::Float64 max)
{
    return min + ((max - min) * ((types::Float64)rand64() / (types::Float64)RAND_MAX));
}

void DRand::MersenneRandom64::DiscardRandom(types::UInt64 nTimes)
{
    {
        int q = 0;
        for (auto i = 0u; i < nTimes; i++)
            q = rand64();
    }
}
