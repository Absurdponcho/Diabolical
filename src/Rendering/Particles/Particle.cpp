#include "Particle.h"
#include <gl/glew.h>
#include "../SpriteRendererComponent.h"
#include <glm/gtc/type_ptr.hpp>
#include "../../Assets/TextAsset.h"

unsigned int ParticleManager::VertexBufferObject = 0;
unsigned int ParticleManager::VertexArrayObject = 0;
unsigned int ParticleManager::ElementBufferObject = 0;
unsigned int ParticleManager::ShaderProgram = 0;

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
    // Vertex Shader =========================================
        TextAsset* VertexShaderSource = GameAssetSoftPointer<TextAsset>("GameAssetFiles/Shaders/ParticleVertex.glsl").LoadSynchronous();
        Check(VertexShaderSource);
        const char* VertexShaderCString = VertexShaderSource->GetString().c_str();

        unsigned int VertexShader;
        VertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(VertexShader, 1, &VertexShaderCString, NULL);
        glCompileShader(VertexShader);

        int Success;
        char InfoLog[512];
        glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);

        if (!Success)
        {
            glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
            Logging::LogError("SpriteRendererComponent::OnSpawn()", "Vertex Shader: " + std::string(InfoLog));
            return;
        }
    //!Vertex Shader =========================================

    // Fragment Shader =======================================
        TextAsset* FragmentShaderSource = GameAssetSoftPointer<TextAsset>("GameAssetFiles/Shaders/ParticleFragment.glsl").LoadSynchronous();
        Check(FragmentShaderSource);
        const char* FragmentShaderCString = FragmentShaderSource->GetString().c_str();

        unsigned int FragmentShader;
        FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(FragmentShader, 1, &FragmentShaderCString, NULL);
        glCompileShader(FragmentShader);

        glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);

        if (!Success)
        {
            glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
            Logging::LogError("SpriteRendererComponent::OnSpawn()", "Fragment Shader: " + std::string(InfoLog));
            return;
        }
    //!Fragment Shader =======================================

    // Shader Program ========================================
        ShaderProgram = glCreateProgram();

        glAttachShader(ShaderProgram, VertexShader);
        glAttachShader(ShaderProgram, FragmentShader);
        glLinkProgram(ShaderProgram);

        glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
        if (!Success) {
            glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
            Logging::LogError("SpriteRendererComponent::OnSpawn()", "Shader Program: " + std::string(InfoLog));
        }

    //!Shader Program ========================================

    glDeleteShader(VertexShader); // delete shaders after shader program is created
    glDeleteShader(FragmentShader);

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
    glm::mat4x4 TranslateMatrix = glm::translate(glm::mat4x4(1.0f), Particle.Position);
    glm::mat4x4 ParticleModelMatrix = glm::scale(TranslateMatrix, glm::vec3(Particle.Size));

    glm::mat4 ViewMatrix = Camera.GetViewMatrix();
    glm::mat4x4 ProjectionMatrix = Camera.GetProjectionMatrix();

    glm::mat4x4 MVPMatrix = ProjectionMatrix * ViewMatrix * ParticleModelMatrix;

    // Bind shader & VAO so opengl knows what to draw
    glUseProgram(ShaderProgram);
    glBindVertexArray(VertexArrayObject);

    // Upload matrix to shader
    int MVPMatrixLocation = glGetUniformLocation(ShaderProgram, "MVP");
    glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

    int ParticleColorLocation = glGetUniformLocation(ShaderProgram, "ParticleColor");
    glUniform4fv(ParticleColorLocation, 1, glm::value_ptr(Particle.Color));

    // draw elements, we have 6 elements so specify 6. For meshes we would
    // need a dynamic element count but sprites will always have 6 elements
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    Particle.Position += glm::vec3(sin(Particle.Rotation), cos(Particle.Rotation), 0) * DeltaTime * Particle.Speed;

    BatchedTransformMatrices[BatchIndex] = MVPMatrix;
    BatchedColors[BatchIndex] = Particle.Color;
}