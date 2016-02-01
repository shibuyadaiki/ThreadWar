#pragma once
#include "../../Math/Vector3.h"
#include <map>
#include <vector>

//�`�`�a�a�̃p�����[�^
struct CubeParameter{
	Vector3 startPos;	//����
	Vector3 endPos;		//�E��
};

//�O�p�`�̃p�����[�^
struct ModelTriangle
{
	Vector3 v1;	//�e���_
	Vector3 v2;	//�e���_
	Vector3 v3;	//�e���_
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