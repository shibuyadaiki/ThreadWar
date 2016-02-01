#pragma once
#include "../../Other/DX11User.h"
#include "../../Other/Device.h"

class LoadTexture{
public:
	LoadTexture();
	~LoadTexture();
	ID3D11ShaderResourceView* LoadBindTexture(const char* texName);

};
