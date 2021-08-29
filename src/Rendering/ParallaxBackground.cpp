#include "ParallaxBackground.h"
#include "../GunGame.h"
#include "../Textures/TextureAsset.h"
#include <gl/glew.h>
#include "../Logging/Logging.h"
#include "../Assets/TextAsset.h"
#include "../Assets/GameAssetSoftPointer.h"
#include "Framebuffer.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>

GLuint ParallaxBackground::ShaderProgram;
GLuint ParallaxBackground::VertexBufferObject;
GLuint ParallaxBackground::VertexArrayObject;
GLuint ParallaxBackground::ElementBufferObject;

float ParallaxVertices[] = {
     1.f,  1.f, 0.0f,   1.0f, 0.0f, // top right
     1.f, -1.f, 0.0f,   1.0f, 1.0f, // bottom right
    -1.f, -1.f, 0.0f,   0.0f, 1.0f, // bottom left
    -1.f,  1.f, 0.0f,   0.0f, 0.0f // top left
};


unsigned int ParallaxIndices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

void ParallaxBackground::Draw(CameraComponent* Camera)
{
    if (!Camera) return;
    glUseProgram(ShaderProgram);
    glBindVertexArray(VertexArrayObject);
	for (BackgroundContainer& BackgroundContainer : BackgroundOrder)
	{
		glBindTexture(GL_TEXTURE_2D, BackgroundContainer.Texture->GetTexture());
       

        int Location = glGetUniformLocation(ShaderProgram, "RenderDepth");
        glUniform1f(Location, BackgroundContainer.RenderDepth);
        
        Location = glGetUniformLocation(ShaderProgram, "CameraPosition");
        glUniform3fv(Location, 1, glm::value_ptr(Camera->GetParentEntity().GetTransform().GetPosition())); 

        Location = glGetUniformLocation(ShaderProgram, "XCameraOffsetMultiplier");
        glUniform1f(Location, BackgroundContainer.XCameraOffsetMultiplier);

        Location = glGetUniformLocation(ShaderProgram, "YOffset");
        glUniform1f(Location, BackgroundContainer.YOffset);

        Location = glGetUniformLocation(ShaderProgram, "AspectRatio");
        glUniform1f(Location, Camera->GetAspectRatio());

        Location = glGetUniformLocation(ShaderProgram, "Scale");
        glUniform2fv(Location, 1, glm::value_ptr(BackgroundContainer.Scale));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void ParallaxBackground::Initialize()
{
	InitializeShader();
	Check(!bInitialized);
	bInitialized = true;

    BackgroundContainer NewBackground = BackgroundContainer();
    NewBackground.Texture = GameAssetSoftPointer<TextureAsset>("GameAssetFiles/ForestTilesetLite/sky.png").LoadSynchronous();
    glBindTexture(GL_TEXTURE_2D, NewBackground.Texture->GetTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    Check(NewBackground.Texture);
    NewBackground.RenderDepth = 0.99999f;
    NewBackground.XCameraOffsetMultiplier = 0;
    NewBackground.YOffset = 0;
    NewBackground.Scale = glm::vec2(.5f, 1);
    BackgroundOrder.push_back(NewBackground);

    NewBackground = BackgroundContainer();
    NewBackground.Texture = GameAssetSoftPointer<TextureAsset>("GameAssetFiles/ForestTilesetLite/mountain2.png").LoadSynchronous();
    glBindTexture(GL_TEXTURE_2D, NewBackground.Texture->GetTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    Check(NewBackground.Texture);
    NewBackground.RenderDepth = 0.99998f;
    NewBackground.XCameraOffsetMultiplier = 0.001f;
    NewBackground.YOffset = -0.2f;
    NewBackground.Scale = glm::vec2(.5f, 1);
    BackgroundOrder.push_back(NewBackground);

    NewBackground = BackgroundContainer();
    NewBackground.Texture = GameAssetSoftPointer<TextureAsset>("GameAssetFiles/ForestTilesetLite/pine1.png").LoadSynchronous();
    glBindTexture(GL_TEXTURE_2D, NewBackground.Texture->GetTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    Check(NewBackground.Texture);
    NewBackground.RenderDepth = 0.99997f;
    NewBackground.XCameraOffsetMultiplier = 0.002f;
    NewBackground.YOffset = -0.4f;
    NewBackground.Scale = glm::vec2(.5f, 1);
    BackgroundOrder.push_back(NewBackground);

    NewBackground = BackgroundContainer();
    NewBackground.Texture = GameAssetSoftPointer<TextureAsset>("GameAssetFiles/ForestTilesetLite/pine2.png").LoadSynchronous();
    glBindTexture(GL_TEXTURE_2D, NewBackground.Texture->GetTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    Check(NewBackground.Texture);
    NewBackground.RenderDepth = 0.99996f;
    NewBackground.XCameraOffsetMultiplier = 0.003f;
    NewBackground.YOffset = -0.5f;
    NewBackground.Scale = glm::vec2(.5f, 1);
    BackgroundOrder.push_back(NewBackground);
}


void ParallaxBackground::InitializeShader()
{
	static bool bShaderInitialized = false;
	if (bShaderInitialized) return;
	bShaderInitialized = true;

    // Vertex Shader =========================================
    TextAsset* VertexShaderSource = GameAssetSoftPointer<TextAsset>("GameAssetFiles/Shaders/ParallaxVertex.glsl").LoadSynchronous();
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
        Logging::LogError("ParallaxBackground::InitializeShader()", "Vertex Shader: " + std::string(InfoLog));
        return;
    }
    //!Vertex Shader =========================================

    // Fragment Shader =======================================
    TextAsset* FragmentShaderSource = GameAssetSoftPointer<TextAsset>("GameAssetFiles/Shaders/ParallaxFragment.glsl").LoadSynchronous();
    Check(FragmentShaderSource);
    const char* FragmentShaderCString = FragmentShaderSource->GetString().c_str();

    unsigned int FragmentShader;
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderCString, NULL);
    glCompileShader(FragmentShader);

    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);

    if (!Success)
    {
        glGetShaderInfoLog(FragmentShader, 512, NULL, InfoLog);
        Logging::LogError("ParallaxBackground::InitializeShader()", "Fragment Shader: " + std::string(InfoLog));
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
        Logging::LogError("ParallaxBackground::InitializeShader()", "Shader Program: " + std::string(InfoLog));
    }

    //!Shader Program ========================================

    glDeleteShader(VertexShader); // delete shaders after shader program is created
    glDeleteShader(FragmentShader);

    // Vertex Array Object ===================================

        // Generate and bind VAO, which stores the VBO and EBO
    glGenVertexArrays(1, &VertexArrayObject);
    glBindVertexArray(VertexArrayObject);

    // Generate and bind VBO, which stores vertex information
    glGenBuffers(1, &VertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ParallaxVertices), ParallaxVertices, GL_STATIC_DRAW);

    // Generate and bind EBO, which stores element information
    glGenBuffers(1, &ElementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ParallaxIndices), ParallaxIndices, GL_STATIC_DRAW);

    // vertices attribute pointer. location is 0, size is 3, stride is 5, offset is 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // uvs attribute pointer. location is 1, size is 2, stride is 5, offset is 3
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //!Vertex Array Object ===================================
}