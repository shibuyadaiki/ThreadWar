#pragma once
#include "OctreeUser.h"
#include "../../Other/DX11User.h"
class OctreeLoader{
public:
	OctreeLoader();
	~OctreeLoader();
	
	//octƒtƒ@ƒCƒ‹‚ğ“Ç‚İ‚İOctreeUser‚É•Û‘¶‚·‚é
	HRESULT Load(const char* modelName, OctreeUser* o);


private:
};