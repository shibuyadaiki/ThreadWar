#pragma once
#include "OctreeUser.h"
#include "../../Other/DX11User.h"
class OctreeLoader{
public:
	OctreeLoader();
	~OctreeLoader();
	
	//oct�t�@�C����ǂݍ���OctreeUser�ɕۑ�����
	HRESULT Load(const char* modelName, OctreeUser* o);


private:
};