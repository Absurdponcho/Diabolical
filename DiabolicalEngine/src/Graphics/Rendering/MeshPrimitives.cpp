#include "MeshPrimitives.h"

std::shared_ptr<DMesh> MeshPrimitives::Cube = std::make_shared<DMesh>(
	DVector<Vector3>({{.5,-.5,-.5}, {.5, -.5, .5}, {-.5, -.5, .5}, {-.5, -.5, -.5}, {.5, .5, -.5}, {.5, .5, .5}, {-.5, .5, .5}, {-.5, .5, -.5}}), // vertex
	DVector<Vector3>({{0, -1, 0}, {0, 1, 0}, {1, 0, 0}, {0, 0, 1}, {-1, 0, 0}, {0, 0, -1}, {1, 0, 0}}), // normals
	DVector<int>({2,1,1,3,2,1,4,3,1,  5,3,2,8,4,2,7,1,2,  5,3,3,6,4,3,2,1,3,  2,2,4,6,3,4,7,4,4,  7,3,5,8,4,5,4,1,5,  5,4,6,1,1,6,4,2,6,  1,4,1,2,1,1,4,3,1,  6,2,2,5,3,2,7,1,2,  1,2,7,5,3,7,2,1,7,  3,1,4,2,2,4,7,4,4,  3,2,5,7,3,5,4,1,5,  8,3,6,5,4,6,4,2,6}), // indices
	DVector<Vector2>({{0, 0}, {1, 0}, {1, 1}, {0, 1}}) // UVs
	);


std::shared_ptr<DMesh> MeshPrimitives::Triangle = std::make_shared<DMesh>(
	DVector<Vector3>({{-.5, -.5, 0}, {.5, -.5, 0}, {0, .5, 0}}),
	DVector<Vector3>(),
	DVector<int>({0, 1, 2}),
	DVector<Vector2>()
	);