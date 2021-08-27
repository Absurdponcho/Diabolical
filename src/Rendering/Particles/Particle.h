#pragma once
#include <glm/glm.hpp>
#include "../Camera.h"

#define PARTICLE_LIMIT 1000

struct Particle
{
	glm::vec4 Color; // R,G,B,A
	glm::vec3 Position;
	float Speed;
	float Rotation; //not using rotation for now, except for movement
	float Size;
	float Lifetime = 1;
};

class ParticleManager
{
public:
	static void Initialize();
	static void BatchParticle(CameraComponent& Camera, Particle& Particle, float DeltaTime, int BatchIndex);
	static void RenderParticles(CameraComponent& Camera, float DeltaTime);
	static void RegisterParticle(const Particle& Particle);
private:
	static glm::mat4 BatchedTransformMatrices[PARTICLE_LIMIT];
	static glm::vec4 BatchedColors[PARTICLE_LIMIT];
	static unsigned int ShaderProgram;
	static unsigned int VertexBufferObject;
	static unsigned int VertexArrayObject;
	static unsigned int ElementBufferObject;
	static unsigned int MVPBufferObject;
	static unsigned int ColorBufferObject;
	static std::vector<Particle> ActiveParticles;
};