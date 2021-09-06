#include "SpriteRendererComponent.h"
#include "../WindowManager.h"
#include "../ECS/GameEntity.h"
#include "../Logging/Logging.h"
#include "../Utility/Utility.h"
#include "../GameManager.h"
#include "../Rendering/Camera.h"
#include <glm/glm.hpp>
#include <gl/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "../Assets/GameAssetSoftPointer.h"
#include "../Assets/GameAsset.h"
#include "../Assets/TextAsset.h"
#include "../Textures/TextureAsset.h"
#include "Particles/Particle.h"
#include "ShaderCompiler.h"

bool SpriteRendererComponent::bGLInitialized = false;
unsigned int SpriteRendererComponent::VertexBufferObject = 0;
unsigned int SpriteRendererComponent::VertexArrayObject = 0;
unsigned int SpriteRendererComponent::ElementBufferObject = 0;
unsigned int SpriteRendererComponent::ShaderProgram = 0;
int SpriteRendererComponent::MVPMatrixLocation = 0;
int SpriteRendererComponent::SpriteDimensionsLocation = 0;
int SpriteRendererComponent::TextureScaleLocation = 0;
int SpriteRendererComponent::DepthLocation = 0;
// Verts for a square of size 1x1
// layout is:
// vertices (x,y,z) then uv (x,y)
float Vertices[] = {
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
     0.5f, -0.5f, 0.0f,   1.0f, 1.0f,   // top right
    -0.5f, -0.5f, 0.0f,   0.0f, 1.0f,   // top left
    -0.5f,  0.5f, 0.0f,   0.0f, 0.0f    // bottom left 
};

unsigned int Indices[] = {  
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

void SpriteRendererComponent::OnDestroy()
{
    GameRendererComponent::OnDestroy();
}

void SpriteRendererComponent::OnSpawn()
{
    GameRendererComponent::OnSpawn();

    if (OpenGLTexture == 0)
    {
        GameAssetSoftPointer<TextureAsset> AssetPointer("GameAssetFiles/InvalidTexture.png");
        SetTexture(AssetPointer);
    }

    if (!bGLInitialized)
    {
        bGLInitialized = true;

        ShaderProgram = ShaderCompiler::CompileShaderProgram(
            GameAssetSoftPointer<TextAsset>("GameAssetFiles/Shaders/SpriteVertex.glsl"),
            GameAssetSoftPointer<TextAsset>("GameAssetFiles/Shaders/SpriteFragment.glsl"));
        Check(ShaderProgram);

        MVPMatrixLocation = glGetUniformLocation(ShaderProgram, "MVP");
        SpriteDimensionsLocation = glGetUniformLocation(ShaderProgram, "SpriteDimensions");
        TextureScaleLocation = glGetUniformLocation(ShaderProgram, "TextureScale");
        DepthLocation = glGetUniformLocation(ShaderProgram, "Depth");

        // Vertex Array Object ===================================

            // Generate and bind VAO, which stores the VBO and EBO
            glGenVertexArrays(1, &VertexArrayObject);
            glBindVertexArray(VertexArrayObject);

            // Generate and bind VBO, which stores vertex information
            glGenBuffers(1, &VertexBufferObject);
            glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

            // Generate and bind EBO, which stores element information
            glGenBuffers(1, &ElementBufferObject);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

            // vertices attribute pointer. location is 0, size is 3, stride is 5, offset is 0
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // uvs attribute pointer. location is 1, size is 2, stride is 5, offset is 3
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

        //!Vertex Array Object ===================================

    }

}

void SpriteRendererComponent::SetTexture(GameAssetSoftPointer<TextureAsset>& Texture)
{
    TextureAsset* TextureAsset = Texture.LoadSynchronous();
    Check(TextureAsset);
    OpenGLTexture = TextureAsset->GetTexture();
}

void SpriteRendererComponent::OnTick(float DeltaTime)
{
    GameComponent::OnTick(DeltaTime);

    if (SpriteSheetProgressionSpeed != 0)
    {
        SelectSpriteSheetIndex(DeltaTime);
    }

}

void SpriteRendererComponent::SelectSpriteSheetIndex(float DeltaTime)
{
    SpriteSheetProgress += DeltaTime * SpriteSheetProgressionSpeed;
    int SpriteSheetIndexer = (int)SpriteSheetProgress;// % (SpriteSheetSize.x * SpriteSheetSize.y);

    SpriteSheetIndex.x = SpriteSheetIndexer % SpriteSheetSize.x;
    SpriteSheetIndex.y = SpriteSheetIndexer / SpriteSheetSize.x;
    if (SpriteSheetIndex.y >= SpriteSheetSize.y)
    {
        SpriteSheetProgress -= (SpriteSheetIndex.y * SpriteSheetSize.x);

        SpriteSheetIndexer = (int)SpriteSheetProgress;
        SpriteSheetIndex.x = SpriteSheetIndexer % SpriteSheetSize.x;
        SpriteSheetIndex.y = SpriteSheetIndexer / SpriteSheetSize.x;
    }

    if (bUseSpriteLoop)
    {
        if (SpriteSheetProgress >= SpriteLoopEnd)
        {
            SpriteSheetProgress = (float)SpriteLoopStart;

            SpriteSheetIndexer = (int)SpriteSheetProgress;
            SpriteSheetIndex.x = SpriteSheetIndexer % SpriteSheetSize.x;
            SpriteSheetIndex.y = SpriteSheetIndexer / SpriteSheetSize.x;
        }

        if (SpriteSheetProgress < SpriteLoopStart)
        {
            SpriteSheetProgress = (float)SpriteLoopStart;

            SpriteSheetIndexer = (int)SpriteSheetProgress;
            SpriteSheetIndex.x = SpriteSheetIndexer % SpriteSheetSize.x;
            SpriteSheetIndex.y = SpriteSheetIndexer / SpriteSheetSize.x;
        }
    }
}

void SpriteRendererComponent::Render(CameraComponent& Camera)
{
    
    glm::mat4x4 ModelMatrix = GetParentEntity()->GetTransform().GetModelMatrix();
    if (bXMirrored)
    {
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(-1, 1, 1));
    }
    if (bYMirrored)
    {
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1, -1, 1));
    }

    glm::mat4 ViewMatrix = Camera.GetViewMatrix();
    glm::mat4x4 ProjectionMatrix = Camera.GetProjectionMatrix();

    glm::mat4x4 MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
    

    // Bind shader & VAO so opengl knows what to draw
    glUseProgram(ShaderProgram);
    glBindVertexArray(VertexArrayObject);

    // Upload matrix to shader
    
    glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

    glUniform4i(SpriteDimensionsLocation, SpriteSheetIndex.x, SpriteSheetIndex.y, SpriteSheetSize.x, SpriteSheetSize.y);
    glUniform2fv(TextureScaleLocation, 1, glm::value_ptr(TextureScale));

    if (bUseYAsDepth)
    {
        glUniform1f(DepthLocation, GetParentEntity()->GetTransform().GetPosition().y / 100000.f);
    }
    else
    {
        glUniform1f(DepthLocation, -69420); // magic number ;) for the shader
    }

    glBindTexture(GL_TEXTURE_2D, OpenGLTexture);
    // draw elements, we have 6 elements so specify 6. For meshes we would
    // need a dynamic element count but sprites will always have 6 elements
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    //Particle Particle;
    //Particle.Color = glm::vec4((float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100, 1);
    //Particle.Position = GetParentEntity()->GetTransform().Position;
    //Particle.Size = 0.1f;
    //Particle.Speed = 5.5f;
    //Particle.Rotation = (float)(rand() % 360);
    //ParticleManager::RegisterParticle(Particle);
}

void SpriteRendererComponent::BindVertexArray()
{
    glBindVertexArray(VertexArrayObject);
}

void SpriteRendererComponent::SetTextureScaling(glm::vec2 Scaling)
{
    TextureScale = Scaling;
}