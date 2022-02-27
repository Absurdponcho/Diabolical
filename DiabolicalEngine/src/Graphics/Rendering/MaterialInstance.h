#pragma once
#include "Material.h"
#include "Types/DVector.h"
#include "Types/DString.h"
#include "Maths/Maths.h"


class DMaterialUniformValueBase 
{
public:
	DMaterialUniformValueBase(GLint NewUniformLocation, const DString& NewName) 
	: UniformLocation(NewUniformLocation), Name(NewName) {}


	DString Name;
	GLint UniformLocation = 0;
	virtual void Apply() {}
};

template <typename T>
class DMaterialUniformValue : public DMaterialUniformValueBase
{
public:
	DMaterialUniformValue(GLint NewUniformLocation, const DString& NewName, const T& NewValue) : 
		DMaterialUniformValueBase(NewUniformLocation, NewName), Value(NewValue) {}

	T Value;

	virtual void Apply() override
	{
		ApplyInternal();
	}

	void ApplyInternal()
	{
		static_assert(false, "You must use a DMaterialUniformValue that has a specific template specialization");
	}
};

template <>
inline void DMaterialUniformValue<Vector4>::ApplyInternal() { glUniform4f(UniformLocation, Value.x, Value.y, Value.z, Value.w); }
template <>
inline void DMaterialUniformValue<Vector3>::ApplyInternal() { glUniform3f(UniformLocation, Value.x, Value.y, Value.z); }
template <>
inline void DMaterialUniformValue<Vector2>::ApplyInternal() { glUniform2f(UniformLocation, Value.x, Value.y); }
template <>
inline void DMaterialUniformValue<IntVector4>::ApplyInternal() { glUniform4i(UniformLocation, Value.x, Value.y, Value.z, Value.w); }
template <>
inline void DMaterialUniformValue<IntVector3>::ApplyInternal() { glUniform3i(UniformLocation, Value.x, Value.y, Value.z); }
template <>
inline void DMaterialUniformValue<IntVector2>::ApplyInternal() { glUniform2i(UniformLocation, Value.x, Value.y); }
template <>
inline void DMaterialUniformValue<int>::ApplyInternal() { glUniform1i(UniformLocation, Value); }
template <>
inline void DMaterialUniformValue<float>::ApplyInternal() { glUniform1f(UniformLocation, Value); }
template <>
inline void DMaterialUniformValue<Matrix4x4>::ApplyInternal() 
{	
	glUniformMatrix4fv(UniformLocation, 1, GL_FALSE, glm::value_ptr((glm::mat4x4)Value)); 
}

class DMaterialInstance
{
public:
	DMaterialInstance(const DSharedPtr<DMaterial>& Material)
		: ParentMaterial(Material) 
	{
		Check(Material->GetProgram() > 0); // The parent material MUST have a valid shader
	}

	inline GLuint GetProgram() { Check(ParentMaterial.Get()) if (!ParentMaterial.Get()) return 0; return ParentMaterial->GetProgram(); }

	template <typename T>
	void SetUniform(const DString& Name, const T& Value)
	{
		Check (Name.Length() > 0);
		if (Name.Length() == 0) return;

		if (DMaterialUniformValue<T>* ExistingUniform = GetUniform<T>(Name))
		{
			ExistingUniform->Value = Value;
			return;
		}

		if (GLuint Program = GetProgram())
		{
			GLint UniformLocation = glGetUniformLocation(Program, *Name);
			if (UniformLocation >= 0)
			{
				MaterialUniformValues.Add(new DMaterialUniformValue<T>(UniformLocation, Name, Value));
			}
		}
	}

	template <typename T>
	DMaterialUniformValue<T>* GetUniform(const DString& Name)
	{
		Check(Name.Length() > 0);
		if (Name.Length() == 0) return nullptr;

		for (DMaterialUniformValueBase* Base : MaterialUniformValues)
		{
			if (Base->Name == Name)
			{
				if (DMaterialUniformValue<T>* Val = dynamic_cast<DMaterialUniformValue<T>*>(Base))
				{
					return Val;
				}
			}
		}

		return nullptr;
	}

	inline void Bind()
	{
		GLuint ShaderProgram = GetProgram();
		glUseProgram(ShaderProgram);
		if (ShaderProgram > 0)
		{
			ApplyAllUniforms();
		}
	}

	inline void ApplyAllUniforms()
	{
		for (DMaterialUniformValueBase* Base : MaterialUniformValues)
		{
			Base->Apply();
		}
	}

protected:
	DSharedPtr<DMaterial> ParentMaterial;
	DVector<DMaterialUniformValueBase*> MaterialUniformValues;
};

