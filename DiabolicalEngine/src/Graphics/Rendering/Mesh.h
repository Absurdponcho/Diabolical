#pragma once
#include "Types/DVector.h"
#include "Maths/Maths.h"
#include "gl/glew.h"
#include <gl/GL.h>

class DMesh
{
public:
	DMesh() {}

	DMesh(const DVector<Vector3>& NewVertices, const DVector<Vector3>& NewNormals, const DVector<int>& NewIndices, const DVector<Vector2>& NewUVs)
		:	Vertices(NewVertices), Normals(NewNormals), Indices(NewIndices), UVs(NewUVs)
	{}

	~DMesh();

	inline const DVector<Vector3>& GetVertices() { return Vertices; }
	inline const DVector<Vector3>& GetNormals() { return Normals; }
	inline const DVector<int>& GetIndices() { return Indices; }
	inline const DVector<Vector2>& GetUVs() { return UVs; }
	inline const GLsizei GetIndexCount() { return (GLsizei)Indices.Size(); }
	inline const GLsizei GetVertexCount() { return (GLsizei)Vertices.Size(); }

	const GLuint GetVertexArray();

	inline void Draw()
	{
		GLuint CurrentVertexArray = GetVertexArray();

		if (!CurrentVertexArray) return;

		GLsizei IndexCount = GetIndexCount();
		GLsizei VertexCount = GetVertexCount();

		if (VertexCount <= 0) return;

		glBindVertexArray(CurrentVertexArray);

		if (IndexCount == 0)
		{
			glDrawArrays(GL_TRIANGLES, 0, GetVertexCount());
		}
		else
		{
			glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, 0);
		}
	}

protected:
	void GenerateBuffers();

	GLuint VertexArray = 0;
	GLuint VertexBuffer = 0;
	GLuint ElementBuffer = 0;

	DVector<Vector3> Vertices;
	DVector<Vector3> Normals;
	DVector<int> Indices;
	DVector<Vector2> UVs;
};

