#include "MeshPrimitives.h"

DSharedPtr<DMesh> MeshPrimitives::Cube = std::make_shared<DMesh>(
	DVector<Vector3>({{-.5, -.5,  0.5}, { .5, -.5,  0.5}, {-.5,  .5,  0.5}, { .5,  .5,  0.5}, {-.5, -.5, -0.5}, {.5, -.5, -0.5}, {-.5,  .5, -0.5}, {.5,  .5, -0.5}}), // vertex
	DVector<Vector3>(), // normals
	DVector<int>({ 2, 6, 7,	2, 3, 7, 0, 4, 5, 0, 1, 5, 0, 2, 6, 0, 4, 6, 1, 3, 7, 1, 5, 7, 0, 2, 3,	0, 1, 3, 4, 6, 7, 4, 5, 7 }), // indices
	DVector<Vector2>() // UVs
	);


DSharedPtr<DMesh> MeshPrimitives::Triangle = std::make_shared<DMesh>(
	DVector<Vector3>({{-.5, -.5, 0}, {.5, -.5, 0}, {0, .5, 0}}),
	DVector<Vector3>(),
	DVector<int>({0, 1, 2}),
	DVector<Vector2>()
	);
