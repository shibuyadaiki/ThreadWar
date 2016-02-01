#pragma once
#include "../../Other/DX11User.h"
#include "RgrALoader.h"
#include "../Shader/Shader.h"
#include "../../Math/Matrix4.h"
#include "../../Actor/ID.h"
#define BONEMAXCOUNT 256
class Animation{
public:
	Animation();
	~Animation();
	//�A�j���[�V�����̃��[�h
	void Load(ANIM_ID id, const char* animName);
	//�A�j���[�V�����̃o�C���h
	void BindAnim(Shader* shader, float animCount);
	//�A�j���[�V�����̃o�C���h
	void BindAnim(Shader* shader, Matrix4* animMat);
	//�A�j���[�V�����̃o�C���h(�u�����h)
	void BindAnimBlend(Animation& otherAni, Shader* shader, float animCount, float otherAnimCount, float blendLevel);
	//�w��t���[���̃A�j���[�V�����𒊏o
	void PopAnim(Matrix4* animMat,AnimUser* animUser, float animCount, bool roop = true);
	void PopAnim(Matrix4* animMat, ANIM_ID id, float animCount, bool roop = true);
	//�u�����h
	void AnimBlend(Matrix4* animMat,ANIM_ID id, float animCount, ANIM_ID otherID, float otherAnimCount, float blendLevel,bool roop = true);
	void AnimBlend(Matrix4* mat, Matrix4* otherMat, float blendLevel, ANIM_ID id);
	Matrix4 AnimBlend(Matrix4 mat, Matrix4 otherMat, float blendLevel);
	AnimUser* ReturnAnimUser(ANIM_ID id);
private:
	RgrALoader rl;
	std::map<ANIM_ID,AnimUser> au;
};