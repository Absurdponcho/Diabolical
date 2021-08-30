#include "TextRendererComponent.h"
#include <gl/glew.h>
#include "../Assets/TextAsset.h"
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

unsigned int TextRendererComponent::VAO;
unsigned int TextRendererComponent::VBO;
unsigned int TextRendererComponent::ShaderProgram;
TextRendererComponent::TextRendererComponent()
{


	static bool bInitialized = false;
	if (!bInitialized)
	{
		bInitialized = true;
		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

        // Vertex Shader =========================================
        TextAsset* VertexShaderSource = GameAssetSoftPointer<TextAsset>("GameAssetFiles/Shaders/TextVertex.glsl").LoadSynchronous();
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
            Logging::LogError("TextRendererComponent::TextRendererComponent()", "Vertex Shader: " + std::string(InfoLog));
            return;
        }
        //!Vertex Shader =========================================

        // Fragment Shader =======================================
        TextAsset* FragmentShaderSource = GameAssetSoftPointer<TextAsset>("GameAssetFiles/Shaders/TextFragment.glsl").LoadSynchronous();
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
            Logging::LogError("TextRendererComponent::TextRendererComponent()", "Fragment Shader: " + std::string(InfoLog));
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
            Logging::LogError("TextRendererComponent::TextRendererComponent()", "Shader Program: " + std::string(InfoLog));
        }

        //!Shader Program ========================================

        glDeleteShader(VertexShader); // delete shaders after shader program is created
        glDeleteShader(FragmentShader);
	}
}

void TextRendererComponent::SetFont(GameAssetSoftPointer<FreetypeFontAsset> FontAssetPointer)
{
	FontAsset = FontAssetPointer.LoadSynchronous();
	Check(FontAsset);
	

	Logging::Log("TextRendererComponent::SetFont()", "Set Text Renderer Font to " + FontAssetPointer.GetPath().string());
}

void TextRendererComponent::Render(CameraComponent& Camera)
{
    RenderText(0, 0, 1);
}

void TextRendererComponent::RenderText(float x, float y, float scale)
{
    glUseProgram(ShaderProgram);
    glUniform3f(glGetUniformLocation(ShaderProgram, "textColor"), Color.x, Color.y, Color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    int ProjectionLocation = glGetUniformLocation(ShaderProgram, "projection");
    glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, glm::value_ptr(CameraComponent::GetActiveCamera()->GetProjectionMatrix()));

    std::string::const_iterator c;
    for (c = Text.begin(); c != Text.end(); c++)
    {
        FreetypeCharacter ch = FreetypeCharacter::Characters[*c];

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] = {
            { 0 + x,     1,   0.0f, 0.0f },
            { 0 + x,     0,       0.0f, 1.0f },
            { 1 + x, 0,       1.0f, 1.0f },

            { 0 + x,     1,   0.0f, 0.0f },
            { 1 + x, 0,       1.0f, 1.0f },
            { 1 + x, 1,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x++;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}