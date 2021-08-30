#include "ShaderCompiler.h"

GLuint ShaderCompiler::CompileShaderProgram(GameAssetSoftPointer<TextAsset> VertexShaderSoftPointer, GameAssetSoftPointer<TextAsset> FragmentShaderSoftPointer)
{
    // Vertex Shader =========================================
    TextAsset* VertexShaderSource = VertexShaderSoftPointer.LoadSynchronous();
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
        Logging::LogError("ShaderCompiler::CompileShaderProgram()", "Vertex Shader: " + std::string(InfoLog));
        return 0;
    }
    //!Vertex Shader =========================================

    // Fragment Shader =======================================
    TextAsset* FragmentShaderSource = FragmentShaderSoftPointer.LoadSynchronous();
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
        Logging::LogError("ShaderCompiler::CompileShaderProgram()", "Fragment Shader: " + std::string(InfoLog));
        glDeleteShader(VertexShader);
        return 0;
    }
    //!Fragment Shader =======================================

    // Shader Program ========================================
    GLuint ShaderProgram = glCreateProgram();

    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
        Logging::LogError("TShaderCompiler::CompileShaderProgram()", "Shader Program: " + std::string(InfoLog));
        glDeleteShader(VertexShader); // delete shaders after shader program is created
        glDeleteShader(FragmentShader);
        return 0;

    }

    //!Shader Program ========================================

    glDeleteShader(VertexShader); // delete shaders after shader program is created
    glDeleteShader(FragmentShader);

    return ShaderProgram;

}