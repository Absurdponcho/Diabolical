#include "Material.h"
#include "Logging/Logging.h"

void DMaterial::BuildShader(const DString& Vertex, const DString& Fragment)
{
	Check (Vertex.Length());
	Check (Fragment.Length());
	
	BuildVertexShader(Vertex);
	BuildFragmentShader(Fragment);

	if (!VertexShader || !FragmentShader)
	{
		DestroyShader();
	}

	BuildProgram();
}

void DMaterial::BuildVertexShader(const DString& Vertex)
{
	const char* VertexShaderCString = *Vertex;

	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexShaderCString, NULL);
	glCompileShader(VertexShader);

	int Success;
	char InfoLog[512];
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);

	if (!Success)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
		LOG_ERR(DString::Format("Vertex Shader: %s", InfoLog));
		VertexShader = 0;
	}
}

void DMaterial::BuildFragmentShader(const DString& Fragment)
{
	const char* FragmentShaderCString = *Fragment;

	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShaderCString, NULL);
	glCompileShader(FragmentShader);

	int Success;
	char InfoLog[512];
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);

	if (!Success)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, InfoLog);
		LOG_ERR(DString::Format("Fragment Shader: %s", InfoLog));
		FragmentShader = 0;
	}
}

void DMaterial::BuildProgram()
{
	ShaderProgram = glCreateProgram();

	Check (VertexShader);
	Check (FragmentShader);
	if (!VertexShader || !FragmentShader)
	{
		// the minimum for a shader program is vertex + fragment
		DestroyShader();
		return;
	}

	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);

	int Success;
	char InfoLog[512];
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
		LOG_ERR(DString::Format("Shader Program: %s", InfoLog));
		ShaderProgram = 0;
	}

	if (VertexShader)
	{
		glDeleteShader(VertexShader);
		VertexShader = 0;
	}
	if (FragmentShader)
	{
		glDeleteShader(FragmentShader);
		FragmentShader = 0;
	}
}

DMaterial::~DMaterial()
{
	DestroyShader();
}

void DMaterial::DestroyShader()
{
	if (VertexShader)
	{
		glDeleteShader(VertexShader);
		VertexShader = 0;
	}

	if (FragmentShader)
	{
		glDeleteShader(FragmentShader);
		FragmentShader = 0;
	}

	if (ShaderProgram)
	{
		glDeleteProgram(ShaderProgram);
		ShaderProgram = 0;
	}
}
