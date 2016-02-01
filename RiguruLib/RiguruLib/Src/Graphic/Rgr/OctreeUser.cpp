#include "OctreeUser.h"

void OctreeUser::SetCubeVec(int cubeCount, CubeParameter c){
	cubeVec[cubeCount].push_back(c);
}
void OctreeUser::SetTriangleInthirdCubeVec(int mesh, int num, ModelTriangle mt){
	TriangleInthirdCubeVec[mesh][num].push_back(mt);
}
std::map<int, std::vector<CubeParameter>>* OctreeUser::ReturnCubeVec(){
	return &cubeVec;
}
std::map<int, std::map<int, std::vector<ModelTriangle>>>* OctreeUser::ReturnTriangleInthirdCubeVec(){
	return &TriangleInthirdCubeVec;
}