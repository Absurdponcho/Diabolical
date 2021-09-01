#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class FastRandom {
public:
	FastRandom();
	FastRandom(int Seed);
	void SetSeed(int NewSeed);
	inline int GetNextRandom();
	inline int GetNextRandom(int Low, int High);
private:
	int Seed;
};

class GlobalRandom {
	GlobalRandom() {};
	GlobalRandom(int Seed);
	// Don't Change Unless you want different LCG engine settings
	static const unsigned int Modulus = 0x7FFFFFFF;
	static const unsigned int Multiplier = 48271;
	static const unsigned int Increment = 0;
public:
	static void Initialize();
	static void Initialize(int Seed);
	static unsigned int Next();
	void Discard(unsigned int z);
	static GlobalRandom* Get();


private:
	static GlobalRandom* Singleton;
	static unsigned int GlobalSeed;
};

class GameMath {
public:
	static inline glm::vec2 RotateVector2D(glm::vec2 Vector, float DegreesAngle)
	{
		glm::mat4 Matrix = glm::mat4_cast(glm::quat(glm::vec3(0, 0, DegreesAngle * 0.0174533f)));
		glm::vec4 TransformedVector = Matrix * glm::vec4(Vector, 0, 1);
		return glm::vec2(TransformedVector.x, TransformedVector.y);
	}
};