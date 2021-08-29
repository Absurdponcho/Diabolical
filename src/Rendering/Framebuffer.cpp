#include "Framebuffer.h"
#include "../GunGame.h"
#include "../Logging/Logging.h"
#include "../Assets/TextAsset.h"
#include "../Assets/GameAssetSoftPointer.h"
#include "SpriteRendererComponent.h"
#include "..\GameManager.h"

GLuint Framebuffer::ShaderProgram;
GLuint Framebuffer::VertexBufferObject;
GLuint Framebuffer::VertexArrayObject;
GLuint Framebuffer::ElementBufferObject;

float FramebufferVertices[] = {
     1.f,  1.f, 0.0f,   1.0f, 1.0f, 
     1.f, -1.f, 0.0f,   1.0f, 0.0f,  
    -1.f, -1.f, 0.0f,   0.0f, 0.0f,  
    -1.f,  1.f, 0.0f,   0.0f, 1.0f   
};


unsigned int FramebufferIndices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Clear()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::Draw()
{

    glBindVertexArray(VertexArrayObject);
    glUseProgram(ShaderProgram);
    glBindTexture(GL_TEXTURE_2D, RenderedTexture);
    int TimeLocation = glGetUniformLocation(ShaderProgram, "Time");
    glUniform1f(TimeLocation, GameManager::GetTime());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

Framebuffer::Framebuffer(int NewWidth, int NewHeight) 
	: Width(NewWidth), Height(NewHeight)
{
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	glGenTextures(1, &RenderedTexture);

	glBindTexture(GL_TEXTURE_2D, RenderedTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, NewWidth, NewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenRenderbuffers(1, &DepthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, NewWidth, NewHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthRenderBuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, RenderedTexture, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	Check(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Logging::Log("Framebuffer::Framebuffer()", "New Framebuffer generated with dimensions " + std::to_string(NewWidth) + "x" + std::to_string(NewHeight));

	Initialize();
		
}
void Framebuffer::Initialize()
{
	static bool bInitialized = false;
	if (bInitialized) return;

	bInitialized = true;

    // Vertex Shader =========================================
    TextAsset* VertexShaderSource = GameAssetSoftPointer<TextAsset>("GameAssetFiles/Shaders/SimpleTextureVertex.glsl").LoadSynchronous();
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
        Logging::LogError("Framebuffer::Initialize()", "Vertex Shader: " + std::string(InfoLog));
        return;
    }
    //!Vertex Shader =========================================

    // Fragment Shader =======================================
    TextAsset* FragmentShaderSource = GameAssetSoftPointer<TextAsset>("GameAssetFiles/Shaders/SimpleTextureFragment.glsl").LoadSynchronous();
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
        Logging::LogError("Framebuffer::Initialize()", "Fragment Shader: " + std::string(InfoLog));
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
        Logging::LogError("Framebuffer::Initialize()", "Shader Program: " + std::string(InfoLog));
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(FramebufferVertices), FramebufferVertices, GL_STATIC_DRAW);

        // Generate and bind EBO, which stores element information
        glGenBuffers(1, &ElementBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(FramebufferIndices), FramebufferIndices, GL_STATIC_DRAW);

        // vertices attribute pointer. location is 0, size is 3, stride is 5, offset is 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // uvs attribute pointer. location is 1, size is 2, stride is 5, offset is 3
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

    //!Vertex Array Object ===================================
}