#pragma once
#include "Types/DVector.h"
#include "Maths/Maths.h"

class DMesh
{
public:
	DMesh() {}

	DMesh(const DVector<Vector3>& NewVertices, const DVector<Vector3>& NewNormals, const DVector<int>& NewIndices, const DVector<Vector2>& NewUVs)
		:	Vertices(NewVertices), Normals(NewNormals), Indices(NewIndices), UVs(NewUVs)
	{}

	inline const DVector<Vector3>& GetVertices() { return Vertices; }
	inline const DVector<Vector3>& GetNormals() { return Normals; }
	inline const DVector<int>& GetIndices() { return Indices; }
	inline const DVector<Vector2>& GetUVs() { return UVs; }


protected:
	DVector<Vector3> Vertices;
	DVector<Vector3> Normals;
	DVector<int> Indices;
	DVector<Vector2> UVs;
};

