#pragma once
#include "AnimUser.h"

class RgrALoader{
public:
	RgrALoader();
	~RgrALoader();

	HRESULT Load(const char* modelName, AnimUser* ani);
private:
};