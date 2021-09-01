#include "Particle.h"
#include <gl/glew.h>
#include "../SpriteRendererComponent.h"
#include <glm/gtc/type_ptr.hpp>
#include "../../Assets/TextAsset.h"
#include "../ShaderCompiler.h"

unsigned int ParticleManager::VertexBufferObject = 0;
unsigned int ParticleManager::VertexArrayObject = 0;
unsigned int ParticleManager::ElementBufferObject = 0;
unsigned int ParticleManager::ShaderProgram = 0;
int ParticleManager::MVPMatrixLocation = 0;
int ParticleManager::ParticleColorLocation = 0;
std::vector<Particle> ParticleManager::ActiveParticles;
glm::mat4 ParticleManager::BatchedTransformMatrices[];
glm::vec4 ParticleManager::BatchedColors[];

float ParticleVertices[] = {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
};

unsigned int ParticleIndices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

void ParticleManager::RegisterParticle(const Particle& Particle)
{
    ActiveParticles.push_back(Particle);
}

void ParticleManager::RenderParticles(CameraComponent& Camera, float DeltaTime)
{
    if (ActiveParticles.size() > PARTICLE_LIMIT)
    {
        ActiveParticles.erase(ActiveParticles.begin(), ActiveParticles.begin() + (ActiveParticles.size() - PARTICLE_LIMIT));
    }
    for (int Index = 0; Index < ActiveParticles.size(); Index++)
    {
        BatchParticle(Camera, ActiveParticles[Index], DeltaTime,Index);
    }

}

void ParticleManager::Initialize()
{
    ShaderProgram = ShaderCompiler::CompileShaderProgram(
        GameAssetSoftPointer<TextAsset>("GameAssetFiles/Shaders/ParticleVertex.glsl"),
        GameAssetSoftPointer<TextAsset>("GameAssetFiles/Shaders/ParticleFragment.glsl"));
    Check(ShaderProgram);

    MVPMatrixLocation = glGetUniformLocation(ShaderProgram, "MVP");
    ParticleColorLocation = glGetUniformLocation(ShaderProgram, "ParticleColor");

    // Vertex Array Object ===================================

        // Generate and bind VAO, which stores the VBO and EBO
        glGenVertexArrays(1, &VertexArrayObject);
        Check(VertexArrayObject);
        glBindVertexArray(VertexArrayObject);

        // Generate and bind VBO, which stores vertex information
        glGenBuffers(1, &VertexBufferObject);
        Check(VertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleVertices), ParticleVertices, GL_STATIC_DRAW);

        // Generate and bind VBO, which stores element information
        glGenBuffers(1, &ElementBufferObject);
        Check(ElementBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ParticleIndices), ParticleIndices, GL_STATIC_DRAW);

        // vertices attribute pointer. location is 0, size is 3, stride is 5, offset is 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

    //!Vertex Array Object ===================================

}

void ParticleManager::BatchParticle(CameraComponent& Camera, Particle& Particle, float DeltaTime, int BatchIndex)
{
    if (Particle.Lifetime > 0)
    {
        glm::mat4x4 TranslateMatrix = glm::translate(glm::mat4x4(1.0f), glm::vec3(Particle.Position.x - glm::mod(Particle.Position.x, Particle.Size), Particle.Position.y - glm::mod(Particle.Position.y, Particle.Size), 0));
        //TranslateMatrix *= glm::toMat4(glm::quat(glm::vec3(0, 0, Particle.Rotation)));
        glm::mat4x4 ParticleModelMatrix = glm::scale(TranslateMatrix, glm::vec3(Particle.Size));

        glm::mat4 ViewMatrix = Camera.GetViewMatrix();
        glm::mat4x4 ProjectionMatrix = Camera.GetProjectionMatrix();

        glm::mat4x4 MVPMatrix = ProjectionMatrix * ViewMatrix * ParticleModelMatrix;

        // Bind shader & VAO so opengl knows what to draw
        glUseProgram(ShaderProgram);
        glBindVertexArray(VertexArrayObject);

        // Upload matrix to shader
        
        glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

        
        glUniform4fv(ParticleColorLocation, 1, glm::value_ptr(Particle.Color));

        // draw elements, we have 6 elements so specify 6. For meshes we would
        // need a dynamic element count but sprites will always have 6 elements
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        Particle.Position += glm::vec3(cos(Particle.Rotation), sin(Particle.Rotation), 0) * DeltaTime * Particle.Speed;
        Particle.Lifetime -= DeltaTime;
        Particle.Rotation += Particle.RotationOverTime * DeltaTime;
        Particle.Speed += Particle.Acceleration * DeltaTime;
        if (Particle.Speed <= 0) Particle.Speed = 0;
    }
    BatchedTransformMatrices[BatchIndex] = glm::mat4();
    BatchedColors[BatchIndex] = Particle.Color;
}