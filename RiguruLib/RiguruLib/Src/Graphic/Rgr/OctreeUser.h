#pragma once
#include "../../Math/Vector3.h"
#include <map>
#include <vector>

//ＡＡＢＢのパラメータ
struct CubeParameter{
	Vector3 startPos;	//左下
	Vector3 endPos;		//右上
};

//三角形のパラメータ
struct ModelTriangle
{
	Vector3 v1;	//各頂点
	Vector3 v2;	//各頂点
	Vector3 v3;	//各頂点
};

class OctreeUser{
public:
	OctreeUser(){}
	~OctreeUser(){}
	void SetCubeVec(int cubeCount, CubeParameter c);

	void SetTriangleInthirdCubeVec(int mesh, int num, ModelTriangle mt);

	std::map<int, std::vector<CubeParameter>>* ReturnCubeVec();

	std::map<int, std::map<int, std::vector<ModelTriangle>>>* ReturnTriangleInthirdCubeVec();
private:
	std::map<int, std::vector<CubeParameter>> cubeVec;
	std::map<int, std::map<int, std::vector<ModelTriangle>>> TriangleInthirdCubeVec;
};