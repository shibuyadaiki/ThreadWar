#pragma once
#include "Texture/Texture.h"
#include "Rgr/Mesh.h"
#include "../Actor/ID.h"
#include "../Math/Matrix4.h"
#include "Shader\Shader.h"
#include "Animation\Animation.h"
#include "Shape\Line.h"
#include "Texture\Font.h"
#include "../Math/Vector2.h"
#include "Rgr\OctreeUser.h"
#include "Rgr\OctreeLoader.h"
#include "../Actor/ActorPtr.h"
#include <list>

struct FontParameter{
	FONT_ID id;
	Vector2 pos;
	Vector2 size;
	float range;
	std::string s;
	Vector3 color;
	float alpha;
	bool center;
};

struct AnimParameter{
	ANIM_ID animMat = ANIM_ID::NULL_ANIM;
	float animTime = 0;
	float* updateAnimTime;
	ANIM_ID animMat2 = ANIM_ID::NULL_ANIM;
	float animTime2 = 0;
	float* updateAnimTime2;
	float animBlend = 0;
	bool roopFlag = true;
	float beforeAnimBlend = 0;
	float* beforeAnimBlendTime;
};
class Graphic{
public:
	Graphic();
	~Graphic();
	static Graphic &GetInstance(){
		static Graphic d;
		return d;
	}
	void Release();
	void LoadTexture(TEXTURE_ID id, const char* modelName);
	void DrawTexture(TEXTURE_ID id, const Matrix4* mat);
	/*centerは0.0が左揃え、0.5が中央揃え、1.0が右揃えになる。デフォルトは0.5の中央揃え。
	rectは0.0から1.0でフルサイズの画像を描画する。*/
	void DrawTexture(TEXTURE_ID id, const Vector2 pos, Vector2 size, D3DXCOLOR color, Vector2 center = vector2(0.5f, 0.5f), float left = 0.0f, float top = 0.0f, float right = 1.0f, float bottom = 1.0f, float angle = NULL);
	void LoadMesh(MODEL_ID id, const char* modelName);
	void DrawMesh(MODEL_ID id, const Matrix4* mat, CAMERA_ID cID, D3DXCOLOR* color = NULL, bool alphaFlag = false);
	MeshUser* ReturnMeshUser(MODEL_ID id);

	void LoadFont(FONT_ID id, const char* modelName);
	//std::to_Stringで数値を文字列に変換できます
	void DrawFont(FONT_ID id, const Vector2 pos, Vector2 size, float range, std::string s, const Vector3 color = vector3(0, 0, 0), const float alpha = 1, bool center = false);
	//std::to_Stringで数値を文字列に変換できます
	void DrawFontDirect(FONT_ID id, const Vector2 pos, Vector2 size, float range, std::string s, const Vector3 color = vector3(0, 0, 0), const float alpha = 1, bool center = false);
	//std::to_Stringで数値を文字列に変換できます
	void DrawFont(FONT_ID id, const Matrix4* mat, std::string s, const Vector3 color = vector3(0, 0, 0), const float alpha = 1);

	void DrawAllFont();


	void LoadAnimation(ANIM_ID id, const char* animName);
	void BindAnimation1(ANIM_ID id, float animCount, ANIM_ID otherID = ANIM_ID::NULL_ANIM, float otherAnimCount = 0, float blendLevel = 0.0f);
	void BindAnimation(ActorPtr actor, SHADER_ID id, float frameTime);
	void SetAnimation(ActorPtr actor, ANIM_ID id, float animTime, float* updateAnimTime, ANIM_ID otherID, float otherAnimTime, float* otherUpdateAnimTime, float blendLevel, bool roop, float* beforeAnimBlendSpeed);
	void SetAnimation(ActorPtr actor, ANIM_ID id, float animTime, float* updateAnimTime, bool roop, float* beforeAnimBlendSpeed);
	void DeleteBackAnimation(ActorPtr actor);
	void LoadShader(SHADER_ID id, const char* shaderName, bool depthEnable = TRUE);
	void SetShader(SHADER_ID id);
	void SetTechniquePass(SHADER_ID id, LPCSTR techniqueName, LPCSTR passName);

	void DrawLine(const Vector3& startPos, const Vector3& endPos, CAMERA_ID cID, const Vector3 color = vector3(1, 1, 1), const float alpha = 1);
	void DrawSphere(const Vector3 pos, const float radius, CAMERA_ID cID, const Vector3 color = vector3(1, 1, 1), const float alpha = 1);
	void DrawCube(const Vector3 startPos, const Vector3 endPos, CAMERA_ID cID, const Vector3 color = vector3(1, 1, 1), const float alpha = 1);
	void DrawCube(const Matrix4 mat, const Vector3 scale, CAMERA_ID cID, const Vector3 color = vector3(1, 1, 1), const float alpha = 1);

	void LoadOctree(OCT_ID id, const char* octName);
	OctreeUser* ReturnOctree(OCT_ID id);
private:
	std::map<TEXTURE_ID, Texture> texture;
	std::map<MODEL_ID, Mesh> mesh;
	std::map<FONT_ID, Font> font;
	Animation animation;
	std::map<SHADER_ID, Shader> shader;
	std::map<OCT_ID, OctreeUser> octree;
	SHADER_ID sID;
	bool animBind;
	Line line;
	std::vector<FontParameter> fontParamVec;
	std::map<ActorPtr, std::list<AnimParameter*>> animParameter;
};