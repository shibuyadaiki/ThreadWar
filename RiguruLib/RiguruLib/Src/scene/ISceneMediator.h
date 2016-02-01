#pragma once

#include "Scene.h"

class ISceneMediator
{
public:
	virtual ~ISceneMediator(){}
	
	virtual void Change(Scene name) = 0;
	virtual void Init(Scene name) = 0;
	virtual void Final(Scene name) = 0;

	virtual int GetStageCount() const = 0;
	virtual void SetStageCount(int n) = 0;

	//__declspec(property(get = GetStageCount, put = SetStageCount)) int StageCount;
};