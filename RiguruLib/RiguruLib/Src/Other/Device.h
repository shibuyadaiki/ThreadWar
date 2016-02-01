#pragma once
#include <map>
#include "DX11User.h"
#include "D3D11User.h"
#include "../Actor/Camera/Camera.h"
#include "../Input/Input.h"
#include "../Actor/ID.h"

class Device{
public:
	Device(){
	}
	~Device(){
	}
	static Device &GetInstance(){
		static Device d;
		return d;
	}

	void Initialize(){
		input = new Input();
		d3d11User = new D3D11USER();
	}

	void CameraInit(CAMERA_ID id, std::weak_ptr<Stage> stage){
		if (camera[id] != NULL){
			SAFE_DELETE(camera[id]);
		}
			camera[id] = new Camera(id, stage);
	}

	void ClearAll(){
		for (auto& i:camera)SAFE_DELETE(i.second);
		SAFE_DELETE(input);

		d3d11User->Invalidate();
		SAFE_DELETE(d3d11User);
	}

	Camera* GetCamera(CAMERA_ID id){
		return camera[id];
	}

	Input* GetInput(){
		return input;
	}

	D3D11USER* Getd3d11User(){
		return d3d11User;
	}
private:
	std::map<CAMERA_ID, Camera*> camera;
	Input* input;
	D3D11USER* d3d11User;
};