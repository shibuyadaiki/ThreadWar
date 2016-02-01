#pragma once
#include "MeshUser.h"

class RgrLoader{
public:
	RgrLoader();
	~RgrLoader();

	HRESULT Load(const char* modelName,MeshUser* m);


private:
	int childCount;
	std::map<int, std::string> texName;
};