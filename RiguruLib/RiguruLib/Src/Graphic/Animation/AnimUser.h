#pragma once
#include "../../Other/DX11User.h"
#include <map>

class AnimUser{
public:
	AnimUser();
	~AnimUser();

	void SetMat(int boneCount, D3DXMATRIX mat);
	void SetAnimFlag(bool f);

	std::map<int, std::vector<D3DXMATRIX>>* ReturnMat();
	bool ReturnAnimFlag();
private:
	//マトリックス
	std::map<int, std::vector<D3DXMATRIX>> mat;
	bool animFlag;
};