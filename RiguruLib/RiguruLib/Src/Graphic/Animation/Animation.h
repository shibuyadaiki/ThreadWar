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
	//アニメーションのロード
	void Load(ANIM_ID id, const char* animName);
	//アニメーションのバインド
	void BindAnim(Shader* shader, float animCount);
	//アニメーションのバインド
	void BindAnim(Shader* shader, Matrix4* animMat);
	//アニメーションのバインド(ブレンド)
	void BindAnimBlend(Animation& otherAni, Shader* shader, float animCount, float otherAnimCount, float blendLevel);
	//指定フレームのアニメーションを抽出
	void PopAnim(Matrix4* animMat,AnimUser* animUser, float animCount, bool roop = true);
	void PopAnim(Matrix4* animMat, ANIM_ID id, float animCount, bool roop = true);
	//ブレンド
	void AnimBlend(Matrix4* animMat,ANIM_ID id, float animCount, ANIM_ID otherID, float otherAnimCount, float blendLevel,bool roop = true);
	void AnimBlend(Matrix4* mat, Matrix4* otherMat, float blendLevel, ANIM_ID id);
	Matrix4 AnimBlend(Matrix4 mat, Matrix4 otherMat, float blendLevel);
	AnimUser* ReturnAnimUser(ANIM_ID id);
private:
	RgrALoader rl;
	std::map<ANIM_ID,AnimUser> au;
};