#pragma once
#include "ID.h"
#include "../math/Matrix4.h"
#include "../math/Vector3.h"
#include "../math/Vector3Utility.h"
#include "../math/MatrixUtility.h"
#include "../Graphic/Rgr/OctreeUser.h"
#include "../Other/DX11User.h"
#include <memory>

//あたり判定のパラメータ
struct CollisionParameter{
	COL_ID id;
	bool colFlag;		//当ったかどうかのフラグ
	Vector3 colPos;		//当ったポジション
	Vector3 colNormal;	//当った面の法線
	bool enemyFlag = false;		//敵からの攻撃（罠）か？
};

//ＯＢＢ
struct OBB{
	D3DXVECTOR3 m_Pos;							// 位置
	std::vector<D3DXVECTOR3> m_NormaDirect;		// 方向ベクトル
	std::vector<FLOAT> m_fLength;				// 各軸方向の長さ
};

//カプセル
struct Capsule{
	Vector3 startPos;	//カプセルのスタートポジション
	Vector3 endPos;		//カプセルのエンドポジション
	float radius;		//カプセルの半径
};

//モデルの三角形を一モデル分全部入れたもの
typedef std::map<int, std::map<int, std::vector<ModelTriangle>>> ModelTriangleVec;
//モデルの三角形を一モデル分全部入れる物をモデルごとにまとめたもの
typedef std::map<int, std::vector<ModelTriangle>> ModelTriangleVecNonMap;

//球とＡＡＢＢのあたり判定
bool ColSphereBox(CubeParameter& aabb, Vector3& spherePos, Matrix4 mat, float& radius);
//レイとＡＡＢＢのあたり判定
bool ColRayBox(Vector3* pos, Vector3* dir_w, CubeParameter* aabb, Matrix4* mat);
//カプセルとＡＡＢＢのあたり判定
bool ColCapsuleBox(Vector3* pos_, Vector3* dir_w_, float radius, CubeParameter* aabb, Matrix4* mat_);

//モデルごとにまとめた三角形とレイを入れるとあたり判定を返すもの。(ContextPrecision＝一番近くを判定するかどうか)
CollisionParameter ModelRay(ModelTriangleVecNonMap* modelTri, Matrix4 mat, Vector3 pos, Vector3 down, bool ContextPrecision = false);
//三角形をとレイを入れるとあたり判定を返すもの。
CollisionParameter ModelRay(ModelTriangle* modelTri, Matrix4 modelMat, Vector3 pos, Vector3 down);
//オクツリーＩＤとそのマトリックスとレイを入れるとあたり判定を返すもの。
CollisionParameter ModelRay(const Matrix4& othermat_, OCT_ID id, Vector3& startPos, Vector3& endPos);
//モデルごとにまとめた三角形と球を入れるとあたり判定を返すもの。(ContextPrecision＝一番近くを判定するかどうか)
CollisionParameter ModelSphere(ModelTriangleVecNonMap* modelTri, Matrix4 mat, Vector3 spherePos, float radius, bool ContextPrecision = false);
//三角形と球を入れるとあたり判定を返すもの。(ContextPrecision＝一番近くを判定するかどうか)
CollisionParameter ModelSphere(ModelTriangle* modelTri, Matrix4 mat, Vector3 spherePos, float radius, bool ContextPrecision = false);
//オクツリーＩＤとそのマトリックスと球を入れるとあたり判定を返すもの。
CollisionParameter ModelSphere(const Matrix4& othermat_, OCT_ID id, Vector3& spherePos, float radius);
//オクツリーＩＤとそのマトリックスとカプセルを入れるとあたり判定を返すもの。
CollisionParameter ModelCapsule(const Matrix4& othermat_, OCT_ID id, Capsule c);

//オクツリーと線分のあたり判定　octNumberを返す
std::shared_ptr<std::vector<int>> OctRay(std::map<int, std::vector<CubeParameter>>* cubeVec, Matrix4 octMat, Vector3 startPos, Vector3 endPos);
//オクツリーとカプセルのあたり判定　octNumberを返す
std::shared_ptr<std::vector<int>> OctCapsule(std::map<int, std::vector<CubeParameter>>* cubeVec, Matrix4 octMat, Vector3 startPos, Vector3 endPos, float radius);
//オクツリーと球のあたり判定　octNumberを返す
std::shared_ptr<std::vector<int>> OctSphere(std::map<int, std::vector<CubeParameter>>* cubeVec, Matrix4 octMat, Vector3 spherePos,float radius);

//ＯＢＢとＯＢＢのあたり判定
bool OBBvsOBB(OBB &obb1,OBB &obb2);
//3つの内積の絶対値の和で投影線分長を計算するもの
FLOAT LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 = 0);
//ＯＢＢ作成
OBB CreateOBB(const Matrix4& mat, const Vector3& scale);

//カプセルとカプセルのあたり判定
CollisionParameter CapsulevsCapsule(Capsule c1, Capsule c2);
//カプセル作成
Capsule CreateCapsule(Vector3 startPos, Vector3 endPos, float radius);

//球とレイのあたり判定
CollisionParameter IsCollideLineToSphere(Vector3 posS, Vector3 posE, Vector3 posSphere, float sphereRadius,Vector3 nor = vector3(0,1,0));

//球と球のあたり判定
CollisionParameter IsCollideSphereToSphere(Vector3 posS, Vector3 posE, float radiusS, float radiusE);

//球とカプセルのあたり判定
CollisionParameter IsCollideCapsuleToSphere(Vector3 posS, Vector3 posE, Vector3 posSphere, float sphereRadius, Vector3 nor = vector3(0, 1, 0));

//三角形と線分のあたり判定
CollisionParameter IsCollideTriangleLine(Vector3 a, Vector3 b, Vector3 c, Vector3 pos, Vector3 down);

CollisionParameter IsCollideTriangleSphere(Vector3 a, Vector3 b, Vector3 c, Vector3 pos, float radiuw);