#include "Collision.h"
#include "../Math/Converter.h"
#include "../Graphic/Graphic.h"
//球とＡＡＢＢのあたり判定
bool ColSphereBox(CubeParameter& aabb, Vector3& spherePos, Matrix4 mat, float& radius){
	Matrix4 invMat = RCMatrix4::inverse(mat);
	Vector3 spherePosA = RCMatrix4::transform(spherePos, invMat);
	Vector3 scale = RCMatrix4::getScale(mat);
	if (aabb.startPos.x <= spherePosA.x && aabb.endPos.x >= spherePosA.x){
		if (aabb.startPos.y <= spherePosA.y && aabb.endPos.y >= spherePosA.y){
			if (aabb.startPos.z <= spherePosA.z && aabb.endPos.z >= spherePosA.z){
				return true;
			}
		}
	}

	float sqLen = 0;
	if (spherePosA.x < aabb.startPos.x){
		sqLen += (spherePosA.x - aabb.startPos.x) * (spherePosA.x - aabb.startPos.x);
	}
	if (spherePosA.x > aabb.endPos.x){
		sqLen += (spherePosA.x - aabb.endPos.x) * (spherePosA.x - aabb.endPos.x);
	}

	if (spherePosA.y < aabb.startPos.y){
		sqLen += (spherePosA.y - aabb.startPos.y) * (spherePosA.y - aabb.startPos.y);
	}
	if (spherePosA.y > aabb.endPos.y){
		sqLen += (spherePosA.y - aabb.endPos.y) * (spherePosA.y - aabb.endPos.y);
	}

	if (spherePosA.z < aabb.startPos.z){
		sqLen += (spherePosA.z - aabb.startPos.z) * (spherePosA.z - aabb.startPos.z);
	}
	if (spherePosA.z > aabb.endPos.z){
		sqLen += (spherePosA.z - aabb.endPos.z) * (spherePosA.z - aabb.endPos.z);
	}
	if (sqrt(sqLen) <= radius / scale.x)return true;

	return false;
}

//レイとＡＡＢＢのあたり判定
bool ColRayBox(Vector3* pos_, Vector3* dir_w_, CubeParameter* aabb, Matrix4* mat_){
	float t;
	float len = RCVector3::length(*dir_w_);
	const D3DXVECTOR3* pos = &RConvert(pos_);
	const D3DXVECTOR3* dir_w = &RConvert(dir_w_);
	const D3DXMATRIX * mat = &RConvert(mat_);
	// 光線を境界ボックスの空間へ移動
	D3DXMATRIX invMat;
	D3DXMatrixInverse(&invMat, 0, mat);
	D3DXVECTOR3 p_l, dir_l;
	D3DXVec3TransformCoord(&p_l, pos, &invMat);
	invMat._41 = 0.0f;
	invMat._42 = 0.0f;
	invMat._43 = 0.0f;
	D3DXVec3TransformCoord(&dir_l, dir_w, &invMat);

	if (aabb->startPos.x <= p_l.x && aabb->endPos.x >= p_l.x){
		if (aabb->startPos.y <= p_l.y && aabb->endPos.y >= p_l.y){
			if (aabb->startPos.z <= p_l.z && aabb->endPos.z >= p_l.z){
				return true;
			}
		}
	}
	D3DXVECTOR3 end = p_l + dir_l;
	if (aabb->startPos.x <= end.x && aabb->endPos.x >= end.x){
		if (aabb->startPos.y <= end.y && aabb->endPos.y >= end.y){
			if (aabb->startPos.z <= end.z && aabb->endPos.z >= end.z){
				return true;
			}
		}
	}

	// 交差判定
	float p[3], d[3], min[3], max[3];
	memcpy(p, &p_l, sizeof(D3DXVECTOR3));
	memcpy(d, &dir_l, sizeof(D3DXVECTOR3));
	memcpy(min, (&aabb->startPos), sizeof(D3DXVECTOR3));
	memcpy(max, (&aabb->endPos), sizeof(D3DXVECTOR3));

	t = -FLT_MAX;
	float t_max = FLT_MAX;

	for (int i = 0; i < 3; ++i) {
		if (abs(d[i]) < FLT_EPSILON) {
			if (p[i] < min[i] || p[i] > max[i]){
				return false; // 交差していない
			}
		}
		else {
			// スラブとの距離を算出
			// t1が近スラブ、t2が遠スラブとの距離
			float odd = 1.0f / d[i];
			float t1 = (min[i] - p[i]) * odd;
			float t2 = (max[i] - p[i]) * odd;
			if (t1 > t2) {
				float tmp = t1; t1 = t2; t2 = tmp;
			}

			if (t1 > t) t = t1;
			if (t2 < t_max) t_max = t2;

			// スラブ交差チェック
			if (t >= t_max)
				return false;
		}
	}
	Vector3 colPos_ = RConvert(&(*pos + t * (*dir_w)));
	
	Vector3 colS = RConvert(&(*pos + t * (*dir_w)));
	Vector3 colE = RConvert(&(*pos + t_max * (*dir_w)));
	float lenP = RCVector3::length(colS - *pos_);
	float lenE = RCVector3::length(colE - (*dir_w_ + *pos_));
	if (len < lenP || len < lenE)return false;
	return true;
}

//カプセルとＡＡＢＢのあたり判定
bool ColCapsuleBox(Vector3* pos_, Vector3* dir_w_,float radius, CubeParameter* aabb, Matrix4* mat_){

	float t;
	float len = RCVector3::length(*dir_w_);
	const D3DXVECTOR3* pos = &RConvert(pos_);
	const D3DXVECTOR3* dir_w = &RConvert(dir_w_);
	const D3DXMATRIX * mat = &RConvert(mat_);

	Vector3 scale = RCMatrix4::getScale(*mat_);
	// 光線を境界ボックスの空間へ移動
	D3DXMATRIX invMat;
	D3DXMatrixInverse(&invMat, 0, mat);
	D3DXVECTOR3 p_l, dir_l;
	D3DXVec3TransformCoord(&p_l, pos, &invMat);
	invMat._41 = 0.0f;
	invMat._42 = 0.0f;
	invMat._43 = 0.0f;
	D3DXVec3TransformCoord(&dir_l, dir_w, &invMat);
	D3DXVECTOR3 end = p_l + dir_l;

	if (aabb->startPos.x <= p_l.x && aabb->endPos.x >= p_l.x){
		if (aabb->startPos.y <= p_l.y && aabb->endPos.y >= p_l.y){
			if (aabb->startPos.z <= p_l.z && aabb->endPos.z >= p_l.z){
				return true;
			}
		}
	}

	if (aabb->startPos.x <= end.x && aabb->endPos.x >= end.x){
		if (aabb->startPos.y <= end.y && aabb->endPos.y >= end.y){
			if (aabb->startPos.z <= end.z && aabb->endPos.z >= end.z){
				return true;
			}
		}
	}
	
	Vector3 startRC, endRC;
	startRC = RConvert(&p_l);
	endRC = RConvert(&end);
	Vector3 aabbCenter = aabb->startPos + (aabb->endPos - aabb->startPos) / 2.0f;
	Vector3 capCenter = startRC + (endRC - startRC) / 2.0f;

	Vector3 capToaabb = aabbCenter - capCenter;

	float range = RCVector3::length(capToaabb) < radius / scale.x ? RCVector3::length(capToaabb) : radius / scale.x;

	startRC += RCVector3::normalize(capToaabb) * range;
	endRC += RCVector3::normalize(capToaabb) * range;

	p_l = RConvert(&startRC);
	dir_l = RConvert(&(endRC - startRC));

	if (aabb->startPos.x <= p_l.x && aabb->endPos.x >= p_l.x){
		if (aabb->startPos.y <= p_l.y && aabb->endPos.y >= p_l.y){
			if (aabb->startPos.z <= p_l.z && aabb->endPos.z >= p_l.z){
				return true;
			}
		}
	}
	
	if (aabb->startPos.x <= end.x && aabb->endPos.x >= end.x){
		if (aabb->startPos.y <= end.y && aabb->endPos.y >= end.y){
			if (aabb->startPos.z <= end.z && aabb->endPos.z >= end.z){
				return true;
			}
		}
	}
	
	// 交差判定
	float p[3], d[3], min[3], max[3];
	memcpy(p, &p_l, sizeof(D3DXVECTOR3));
	memcpy(d, &dir_l, sizeof(D3DXVECTOR3));
	memcpy(min, (&aabb->startPos), sizeof(D3DXVECTOR3));
	memcpy(max, (&aabb->endPos), sizeof(D3DXVECTOR3));
	
	t = -FLT_MAX;
	float t_max = FLT_MAX;
	
	for (int i = 0; i < 3; ++i) {
		if (abs(d[i]) < FLT_EPSILON) {
			if (p[i] < min[i] || p[i] > max[i]){
				return false; // 交差していない
			}
		}
		else {
			// スラブとの距離を算出
			// t1が近スラブ、t2が遠スラブとの距離
			float odd = 1.0f / d[i];
			float t1 = (min[i] - p[i]) * odd;
			float t2 = (max[i] - p[i]) * odd;
			if (t1 > t2) {
				float tmp = t1; t1 = t2; t2 = tmp;
			}
	
			if (t1 > t) t = t1;
			if (t2 < t_max) t_max = t2;
	
			// スラブ交差チェック
			if (t >= t_max)
				return false;
		}
	}
	Vector3 colPos_ = RConvert(&(*pos + t * (*dir_w)));
	
	Vector3 colS = RConvert(&(*pos + t * (*dir_w)));
	Vector3 colE = RConvert(&(*pos + t_max * (*dir_w)));
	float lenP = RCVector3::length(colS - *pos_);
	float lenE = RCVector3::length(colE - (*dir_w_ + *pos_));
	if (len < lenP || len < lenE)return false;
	return true;

	//if (ColRayBox(pos_, dir_w_, aabb, mat_))return true;
	//if (ColSphereBox(*aabb, *pos_, *mat_, radius))return true;
	//if (ColSphereBox(*aabb, (*pos_ + *dir_w_), *mat_, radius))return true;
	//
	//Vector3 leftUnderFront, rightUnderFront, leftUpFront, rightUpFront, leftUnderBack, rightUnderBack, leftUpBack, rightUpBack;
	//leftUnderFront = (*aabb).startPos;
	//rightUnderFront = vector3((*aabb).endPos.x, (*aabb).startPos.y, (*aabb).startPos.z);
	//leftUpFront = vector3((*aabb).startPos.x, (*aabb).endPos.y, (*aabb).startPos.z);
	//rightUpFront = vector3((*aabb).endPos.x, (*aabb).endPos.y, (*aabb).startPos.z);
	//
	//leftUnderBack = vector3((*aabb).startPos.x, (*aabb).startPos.y, (*aabb).endPos.z);
	//rightUnderBack = vector3((*aabb).endPos.x, (*aabb).startPos.y, (*aabb).endPos.z);
	//leftUpBack = vector3((*aabb).startPos.x, (*aabb).endPos.y, (*aabb).endPos.z);
	//rightUpBack = (*aabb).endPos;
	//
	//Matrix4 invMat = RCMatrix4::inverse(*mat_);
	//Vector3 pos = RCMatrix4::transform(*pos_,invMat);
	//Vector3 end = RCMatrix4::transform((*pos_ + *dir_w_),invMat);
	//Capsule c = CreateCapsule(pos,end,radius / RCMatrix4::getScale(*mat_).x);
	//if (CapsulevsCapsule(c, CreateCapsule(leftUnderFront, leftUnderBack,0)).colFlag){
	//	return true;
	//}
	//if (CapsulevsCapsule(c, CreateCapsule(rightUnderFront, rightUnderBack,0)).colFlag){
	//	return true;
	//}					 	
	//if (CapsulevsCapsule(c, CreateCapsule(leftUpFront, leftUpBack,0)).colFlag){
	//	return true;
	//}					 	
	//if (CapsulevsCapsule(c, CreateCapsule(rightUpFront, rightUpBack,0)).colFlag){
	//	return true;
	//}					 	
	//if (CapsulevsCapsule(c, CreateCapsule(leftUnderFront, rightUnderFront,0)).colFlag){
	//	return true;
	//}					 	
	//if (CapsulevsCapsule(c, CreateCapsule(leftUnderFront, leftUpFront,0)).colFlag){
	//	return true;
	//}					 	
	//if (CapsulevsCapsule(c, CreateCapsule(rightUnderFront, rightUpFront,0)).colFlag){
	//	return true;
	//}					 	
	//if (CapsulevsCapsule(c, CreateCapsule(rightUpFront, leftUpFront,0)).colFlag){
	//	return true;
	//}					 	
	//if (CapsulevsCapsule(c, CreateCapsule(leftUnderBack, rightUnderBack,0)).colFlag){
	//	return true;
	//}
	//if (CapsulevsCapsule(c, CreateCapsule(leftUnderBack, leftUpBack,0)).colFlag){
	//	return true;
	//}
	//if (CapsulevsCapsule(c, CreateCapsule(rightUpBack, leftUpBack,0)).colFlag){
	//	return true;
	//}
	//if (CapsulevsCapsule(c, CreateCapsule(rightUnderBack, rightUpBack,0)).colFlag){
	//	return true;
	//}
	//
	//return false;
}

//モデルごとにまとめた三角形とレイを入れるとあたり判定を返すもの。(ContextPrecision＝一番近くを判定するかどうか)
CollisionParameter ModelRay(ModelTriangleVecNonMap* modelTri, Matrix4 mat, Vector3 pos, Vector3 down, bool ContextPrecision){
	//あたり判定のパラメータ
	CollisionParameter colpara;
	//当たってない
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);
	std::vector<CollisionParameter> colparaVec;
	//四角形の中の三角形の数
	std::vector<int>triangleSize;
	//モデルの数だけ回す
	for (auto& i : (*modelTri)){
		for (auto& tr : i.second){
			colpara = ModelRay(&tr, mat, pos, down);
			if (colpara.colFlag)
				colparaVec.push_back(colpara);

		}
	}
	float len = 1000000000;
	for (auto& cV : colparaVec){
		if (len > RCVector3::length(cV.colPos - pos) && cV.colFlag){
			len = RCVector3::length(cV.colPos - pos);
			colpara = cV;
		}
	}
	return colpara;
}

//三角形をとレイを入れるとあたり判定を返すもの。
CollisionParameter ModelRay(ModelTriangle* modelTri, Matrix4 modelMat, Vector3 pos, Vector3 down){
	//あたり判定のパラメータ
	CollisionParameter colpara;
	//当たってない
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);
	std::vector<CollisionParameter> colparaVec;
	//四角形の中の三角形の数
	std::vector<int>triangleSize;
	//三角ポリゴン
	Vector3 a = modelTri->v1 * modelMat;
	Vector3 b = modelTri->v2 * modelMat;
	Vector3 c = modelTri->v3 * modelMat;
	Vector3 nor = RCVector3::CreateNormal(a, b, c);
	//各頂点からレイの上(pos)下(down)へのベクトルを求める
	Vector3 apos = (pos - a);
	Vector3 adown = (down - a);

	Vector3 bpos = (pos - b);
	Vector3 bdown = (down - b);

	Vector3 cpos = (pos - c);
	Vector3 cdown = (down - c);

	//法線の長さを求める
	float norLen = RCVector3::length(nor);

	//各頂点からレイの上と下へのベクトルと法線の内分を求める
	float aposLenA = RCVector3::length(RCVector3::dot(apos, nor) / RCVector3::length(apos) * norLen * apos);
	float adownLenA = RCVector3::length(RCVector3::dot(adown, nor) / RCVector3::length(adown) * norLen * adown);
	float aposAA = aposLenA / (aposLenA + adownLenA);
	Vector3 colVecA = (1 - aposAA) * apos + aposAA * adown;
	Vector3 crossPos = a + colVecA;
	//float aposLenB = RCVector3::length(RCVector3::dot(bpos, nor) / RCVector3::length(bpos) * norLen * bpos);
	//float adownLenB = RCVector3::length(RCVector3::dot(bdown, nor) / RCVector3::length(bdown) * norLen * bdown);
	//float aposAB = aposLenB / (aposLenB + adownLenB);
	Vector3 colVecB = crossPos - b;// (1 - aposAB) * bpos + aposAB * bdown;
	//float aposLenC = RCVector3::length(RCVector3::dot(cpos, nor) / RCVector3::length(cpos) * norLen * cpos);
	//float adownLenC = RCVector3::length(RCVector3::dot(cdown, nor) / RCVector3::length(cdown) * norLen * cdown);
	//float aposAC = aposLenC / (aposLenC + adownLenC);
	Vector3 colVecC = crossPos - c;// (1 - aposAC) * cpos + aposAC * cdown;
	//交点へのベクトルの外積を求め
	Vector3 ac = RCVector3::normalize(RCVector3::cross(colVecA, colVecC));
	Vector3 ab = RCVector3::normalize(RCVector3::cross(colVecB, colVecA));
	Vector3 bc = RCVector3::normalize(RCVector3::cross(colVecC, colVecB));

	//その外積が法線と平行かどうか調べて判定する
	float dot_ac_n = RCVector3::dot(ac, nor);
	float dot_ab_n = RCVector3::dot(ab, nor);
	float dot_bc_n = RCVector3::dot(bc, nor);
	if ((dot_ac_n >= 0.9999f && dot_ac_n <= 1.0001f) &&
		(dot_ab_n >= 0.9999f && dot_ab_n <= 1.0001f) &&
		(dot_bc_n >= 0.9999f && dot_bc_n <= 1.0001f)){
		colpara.colFlag = true;
		colpara.colPos = crossPos;
		colpara.colNormal = nor;

		return colpara;
	}
	return colpara;
}

//オクツリーＩＤとそのマトリックスとレイを入れるとあたり判定を返すもの。
CollisionParameter ModelRay(const Matrix4& othermat_, OCT_ID id, Vector3& startPos, Vector3& endPos){
	CollisionParameter colpara;
	//当たってない
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);
	if (id == OCT_ID::NULL_OCT){
		return colpara;
	}
	//一番小さい四角形
	std::map<int, std::vector<CubeParameter>>* cubeVec;
	//一番小さい四角形の中の三角形
	ModelTriangleVec* modelTri;
	//ステージのマトリックスを宣言＆セット

	//オクツリー情報取得
	OctreeUser* oct = Graphic::GetInstance().ReturnOctree(id);
	//一番小さい四角形セット
	cubeVec = oct->ReturnCubeVec();
	//一番小さい四角形の中の三角形をセット
	modelTri = oct->ReturnTriangleInthirdCubeVec();

	std::shared_ptr<std::vector<int>> firstCount = OctRay(cubeVec, othermat_, startPos, endPos);
	float len = 1000000000;
	for (auto& a : (*firstCount)){
		CollisionParameter cV = ModelRay(&(*modelTri)[a], othermat_, startPos, endPos, true);
		if (len > RCVector3::length(cV.colPos - startPos) && cV.colFlag){
			len = RCVector3::length(cV.colPos - startPos);
			colpara = cV;
		}
	}
	return  colpara;
}

//モデルごとにまとめた三角形と球を入れるとあたり判定を返すもの。(ContextPrecision＝一番近くを判定するかどうか)
CollisionParameter ModelSphere(ModelTriangleVecNonMap* modelTri, Matrix4 mat, Vector3 spherePos, float radius, bool ContextPrecision){
	//あたり判定のパラメータ
	CollisionParameter colpara;
	//当たってない
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);
	std::vector<CollisionParameter> colparaVec;
	//四角形の中の三角形の数
	std::vector<int>triangleSize;
	//モデルの数だけ回す
	for (auto& i : (*modelTri)){
		for (auto& tr : i.second){
			colpara = ModelSphere(&tr, mat, spherePos, radius);
			if (colpara.colFlag)colparaVec.push_back(colpara);
		}
	}

	float len = 1000000000;
	for (auto& cV : colparaVec){
		if (len > RCVector3::length(cV.colPos - spherePos) && cV.colFlag){
			len = RCVector3::length(cV.colPos - spherePos);
			colpara = cV;
		}
	}

	return colpara;
}

//三角形と球を入れるとあたり判定を返すもの。(ContextPrecision＝一番近くを判定するかどうか)
CollisionParameter ModelSphere(ModelTriangle* modelTri, Matrix4 mat, Vector3 spherePos, float radius, bool ContextPrecision){
	//あたり判定のパラメータ
	CollisionParameter colpara;
	//当たってない
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);
	std::vector<CollisionParameter> colparaVec;
	//四角形の中の三角形の数
	std::vector<int>triangleSize;
	//モデルの数だけ回す
	//三角ポリゴン
	Vector3 a = modelTri->v1 * mat;
	Vector3 b = modelTri->v2 * mat;
	Vector3 c = modelTri->v3 * mat;


	static bool testSeeFlag = false;
	/*if (testSeeFlag){
		Graphic::GetInstance().DrawLine(a, b, vector3(1, 0, 0));
		Graphic::GetInstance().DrawLine(b, c, vector3(1, 0, 0));
		Graphic::GetInstance().DrawLine(c, a, vector3(1, 0, 0));
	}*/

	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_F, true))testSeeFlag = !testSeeFlag;


	//法線
	Vector3 nor = -RCVector3::CreateNormal(a, b, c);
	Vector3 sPostoA = RCVector3::normalize(spherePos - a);
	Vector3 vec;
	vec = spherePos - nor * (radius);
	colpara = ModelRay(modelTri, mat, spherePos, vec);
	if (colpara.colFlag){
		colparaVec.push_back(colpara);
	}
	vec = spherePos + nor * (radius);
	colpara = ModelRay(modelTri, mat, spherePos, vec);
	if (colpara.colFlag){
		colparaVec.push_back(colpara);
	}
	float len = 1000000000;
	for (auto& cV : colparaVec){
		if (len > RCVector3::length(cV.colPos - spherePos) && cV.colFlag){
			len = RCVector3::length(cV.colPos - spherePos);
			colpara = cV;
		}
	}
	colpara.colNormal = -nor;
	return colpara;
}

//オクツリーＩＤとそのマトリックスと球を入れるとあたり判定を返すもの。
CollisionParameter ModelSphere(const Matrix4& othermat_, OCT_ID id, Vector3& spherePos, float radius){
	CollisionParameter colpara;
	//当たってない
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);
	if (id == OCT_ID::NULL_OCT){
		return colpara;
	}
	//一番小さい四角形
	std::map<int, std::vector<CubeParameter>>* cubeVec;
	//一番小さい四角形の中の三角形
	ModelTriangleVec* modelTri;
	//ステージのマトリックスを宣言＆セット

	//オクツリー情報取得
	OctreeUser* oct = Graphic::GetInstance().ReturnOctree(id);
	//一番小さい四角形セット
	cubeVec = oct->ReturnCubeVec();
	//一番小さい四角形の中の三角形をセット
	modelTri = oct->ReturnTriangleInthirdCubeVec();
	Matrix4 mat = othermat_;

	/*if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_R)){
		Graphic::GetInstance().DrawSphere(spherePos, radius);
	}*/

	std::shared_ptr<std::vector<int>> firstCount = OctSphere(cubeVec, othermat_, spherePos, radius);

	float len = 1000000000;
	for (auto& a : (*firstCount)){
		CollisionParameter cV = ModelSphere(&(*modelTri)[a], mat, spherePos, radius, true);
		if (len > RCVector3::length(cV.colPos - spherePos) && cV.colFlag){
			len = RCVector3::length(cV.colPos - spherePos);
			colpara = cV;
		}
	}
	return colpara;
}

//オクツリーＩＤとそのマトリックスとカプセルを入れるとあたり判定を返すもの。
CollisionParameter ModelCapsule(const Matrix4& othermat_, OCT_ID id, Capsule c){
	CollisionParameter colpara;
	//当たってない
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);

	if (id == OCT_ID::NULL_OCT){
		return colpara;
	}
	//一番小さい四角形
	std::map<int, std::vector<CubeParameter>>* cubeVec;
	//一番小さい四角形の中の三角形
	ModelTriangleVec* modelTri;
	//ステージのマトリックスを宣言＆セット

	//オクツリー情報取得
	OctreeUser* oct = Graphic::GetInstance().ReturnOctree(id);
	//一番小さい四角形セット
	cubeVec = oct->ReturnCubeVec();
	//一番小さい四角形の中の三角形をセット
	modelTri = oct->ReturnTriangleInthirdCubeVec();

	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_R)){
	//	Graphic::GetInstance().DrawLine(c.startPos, c.endPos, vector3(0, 0, 1));
	//
	//	Graphic::GetInstance().DrawSphere(c.startPos, c.radius);
	//	Graphic::GetInstance().DrawSphere(c.endPos, c.radius);
	//}
	std::shared_ptr<std::vector<int>> firstCount = OctCapsule(cubeVec, othermat_, c.startPos, c.endPos , c.radius);
	std::vector<CollisionParameter> colparaVec; 

	for (auto& a : (*firstCount)){
		for (auto& s : (*modelTri)[a]){
			for (auto& t : s.second){
				colpara = ModelRay(&t, othermat_, c.startPos, c.endPos);
				if (colpara.colFlag){
					return colpara;
				}
				colpara = ModelSphere(&t, othermat_, c.startPos, c.radius, true);
				if (colpara.colFlag){
					return colpara;
				} 
				colpara = ModelSphere(&t, othermat_, c.endPos, c.radius, true);
				if (colpara.colFlag){
					return colpara;
				}
				Capsule c1, c2, c3;
				Vector3 v1 = t.v1 * othermat_;
				Vector3 v2 = t.v2 * othermat_;
				Vector3 v3 = t.v3 * othermat_;
				c1 = CreateCapsule(v1, v2, 0);
				c2 = CreateCapsule(v2, v3, 0);
				c3 = CreateCapsule(v3, v1, 0);
				Vector3 nor = RCVector3::CreateNormal(v1, v2, v3);
				colpara = CapsulevsCapsule(c, c1);
				if (colpara.colFlag){
					colpara.colNormal = nor;
					return colpara;
				}
				colpara = CapsulevsCapsule(c, c2);
				if (colpara.colFlag){
					colpara.colNormal = nor;
					return colpara;
				}
				colpara = CapsulevsCapsule(c, c3);
				if (colpara.colFlag){
					colpara.colNormal = nor;
					return colpara;
				}
			}
		}
	}

	return colpara; 
	float len = 1000000000;
	for (auto& a : colparaVec){
		CollisionParameter cV = a;
		if (len > RCVector3::length(cV.colPos - c.startPos) && cV.colFlag){
			len = RCVector3::length(cV.colPos - c.startPos);
			colpara = cV;
		}
	}

	return colpara;
}

//オクツリーと線分のあたり判定　octNumberを返す
std::shared_ptr<std::vector<int>> OctRay(std::map<int, std::vector<CubeParameter>>* cubeVec, Matrix4 octMat, Vector3 startPos, Vector3 endPos){
	Matrix4 mat = octMat;
	//下向きのライン描画
	Vector3 pos = startPos;
	Vector3 end = endPos;
	std::vector<int> firstCount;
	int count = 0;
	for (auto& cubeFor : (*cubeVec)[0]){
		if (ColRayBox(&pos, &(end - pos), &cubeFor, &mat)){
			firstCount.push_back(count);
		}
		count++;
	}
	for (int i = 1; i < (*cubeVec).size(); i++){
		count = 0;
		std::vector<int> countVec = firstCount;
		firstCount.clear();
		for (auto& f : countVec){
			for (int s = 8 * f; s < 8 * f + 8; s++){
				if (ColRayBox(&pos, &(end - pos), &(*cubeVec)[i][s], &mat)){
					firstCount.push_back(s);
				}
				count++;
			}
		}
	}
	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_R)){
	//	for (auto s : (firstCount)){
	//		Graphic::GetInstance().DrawCube(
	//			(*cubeVec)[(*cubeVec).size() - 1][s].startPos * mat,
	//			(*cubeVec)[(*cubeVec).size() - 1][s].endPos   * mat,
	//			vector3(1, 1, 1),
	//			0.5f);
	//	}
	//}
	return std::make_shared<std::vector<int>>(firstCount);
}

//オクツリーとカプセルのあたり判定　octNumberを返す
std::shared_ptr<std::vector<int>> OctCapsule(std::map<int, std::vector<CubeParameter>>* cubeVec, Matrix4 octMat, Vector3 startPos, Vector3 endPos,float radius){
	Matrix4 mat = octMat;
	//下向きのライン描画
	Vector3 pos = startPos;
	Vector3 end = endPos;
	std::vector<int> firstCount;
	int count = 0;
	for (auto& cubeFor : (*cubeVec)[0]){
		if (ColCapsuleBox(&pos, &(end - pos),radius,&cubeFor, &mat)){
			firstCount.push_back(count);
		}
		count++;
	}
	for (int i = 1; i < (*cubeVec).size(); i++){
		count = 0;
		std::vector<int> countVec = firstCount;
		firstCount.clear();
		for (auto& f : countVec){
			for (int s = 8 * f; s < 8 * f + 8; s++){
				if (ColCapsuleBox(&pos, &(end - pos),radius, &(*cubeVec)[i][s], &mat)){
					firstCount.push_back(s);
				}
				count++;
			}
		}
	}
	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_R)){
	//	for (auto s : (firstCount)){
	//		Graphic::GetInstance().DrawCube(
	//			(*cubeVec)[(*cubeVec).size() - 1][s].startPos * mat,
	//			(*cubeVec)[(*cubeVec).size() - 1][s].endPos   * mat,
	//			vector3(1, 1, 1),
	//			0.5f);
	//	}
	//}
	return std::make_shared<std::vector<int>>(firstCount);
}

//オクツリーと球のあたり判定　octNumberを返す
std::shared_ptr<std::vector<int>> OctSphere(std::map<int, std::vector<CubeParameter>>* cubeVec, Matrix4 octMat, Vector3 spherePos, float radius){
	std::vector<int> firstCount;
	int count = 0;
	for (auto& cubeFor : (*cubeVec)[0]){
		if (ColSphereBox(cubeFor, spherePos, octMat, radius)){
			firstCount.push_back(count);
		}
		count++;
	}
	for (int i = 1; i < (*cubeVec).size(); i++){
		count = 0;
		std::vector<int> countVec = firstCount;
		firstCount.clear();
		for (auto& f : countVec){
			for (int s = 8 * f; s < 8 * f + 8; s++){
				if (ColSphereBox((*cubeVec)[i][s], spherePos, octMat, radius)){
					firstCount.push_back(s);
				}
				count++;
			}
		}
	}

	/*if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_R)){
		for (auto& s : firstCount){
			Graphic::GetInstance().DrawCube(
				(*cubeVec)[(*cubeVec).size() - 1][s].startPos * octMat,
				(*cubeVec)[(*cubeVec).size() - 1][s].endPos   * octMat,
				vector3(1, 1, 1),
				0.5f);
		}
	}*/
	return std::make_shared<std::vector<int>>(firstCount);
}

//ＯＢＢとＯＢＢのあたり判定
bool OBBvsOBB(OBB &obb1, OBB &obb2){
	// 各方向ベクトルの確保
	// （N***:標準化方向ベクトル）
	D3DXVECTOR3 NAe1 = obb1.m_NormaDirect[0], Ae1 = NAe1 * obb1.m_fLength[0];
	D3DXVECTOR3 NAe2 = obb1.m_NormaDirect[1], Ae2 = NAe2 * obb1.m_fLength[1];
	D3DXVECTOR3 NAe3 = obb1.m_NormaDirect[2], Ae3 = NAe3 * obb1.m_fLength[2];
	D3DXVECTOR3 NBe1 = obb2.m_NormaDirect[0], Be1 = NBe1 * obb2.m_fLength[0];
	D3DXVECTOR3 NBe2 = obb2.m_NormaDirect[1], Be2 = NBe2 * obb2.m_fLength[1];
	D3DXVECTOR3 NBe3 = obb2.m_NormaDirect[2], Be3 = NBe3 * obb2.m_fLength[2];
	D3DXVECTOR3 Interval = obb1.m_Pos - obb2.m_Pos;

	// 分離軸 : Ae1
	FLOAT rA = D3DXVec3Length(&Ae1);
	FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	FLOAT L = fabs(D3DXVec3Dot(&Interval, &NAe1));
	if (L > rA + rB)
		return false; // 衝突していない

	// 分離軸 : Ae2
	rA = D3DXVec3Length(&Ae2);
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NAe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Ae3
	rA = D3DXVec3Length(&Ae3);
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NAe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be1);
	L = fabs(D3DXVec3Dot(&Interval, &NBe1));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be2);
	L = fabs(D3DXVec3Dot(&Interval, &NBe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NBe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C12
	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C13
	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C21
	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C22
	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C23
	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C31
	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C32
	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C33
	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離平面が存在しないので「衝突している」
	return true;
}

//3つの内積の絶対値の和で投影線分長を計算するもの
FLOAT LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3){
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	FLOAT r1 = fabs(D3DXVec3Dot(Sep, e1));
	FLOAT r2 = fabs(D3DXVec3Dot(Sep, e2));
	FLOAT r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}

//ＯＢＢ作成
OBB CreateOBB(const Matrix4& mat, const Vector3& scale){
	D3DXVECTOR3 pos1, pos2;
	pos1 = RConvert(&RCMatrix4::getPosition(mat));
	std::vector<D3DXVECTOR3> nor1 = {
		RConvert(&RCMatrix4::getLeft(mat)),
		RConvert(&RCMatrix4::getUp(mat)),
		RConvert(&RCMatrix4::getFront(mat))
	};
	std::vector<float> rad1 = {
		scale.x / 2,
		scale.y / 2,
		scale.z / 2
	};
	return{ pos1, nor1, rad1 };
}

//カプセルとカプセルのあたり判定
CollisionParameter CapsulevsCapsule(Capsule c1, Capsule c2){
	CollisionParameter colpara;
	//当たってない
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);

	Vector3 vec1, vec2;
	vec1 = c1.endPos - c1.startPos;
	vec2 = c2.endPos - c2.startPos;

	Vector3 vec = c1.startPos - c2.startPos;

	Vector3 cross = RCVector3::normalize(RCVector3::cross(vec1, vec2));
	float d = RCVector3::dot(vec, cross);
	float d2 = RCVector3::dot(vec, RCVector3::normalize(vec1));
	Vector3 vec1HitPos = c1.startPos + RCVector3::normalize(vec1) * -d2;

	bool start1 = false;
	bool end1 = false;
	float start1Len = RCVector3::length(vec1HitPos - c1.startPos);
	float end1Len = RCVector3::length(vec1HitPos - c1.endPos);

	if (start1Len > RCVector3::length(vec1)){
		start1 = true;
		vec1HitPos = c1.endPos;
	}
	if (end1Len > RCVector3::length(vec1))	{
		end1 = true;
		vec1HitPos = c1.startPos;
	}

	if (IsCollideCapsuleToSphere(c2.startPos,c2.endPos,vec1HitPos,c1.radius + c2.radius).colFlag){
		colpara.colFlag = true;
		colpara.colPos = vec1HitPos + cross * c1.radius;
		return colpara;
	}

	return colpara;
}

//カプセル作成
Capsule CreateCapsule(Vector3 startPos, Vector3 endPos, float radius){
	Capsule c = { startPos, endPos, radius };
	return c;
}

//球とレイのあたり判定
CollisionParameter IsCollideLineToSphere(Vector3 posS, Vector3 posE, Vector3 posSphere, float sphereRadius, Vector3 nor)
{
	CollisionParameter colpara;
	//当たってない
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = nor;

	Vector3 vSToE, vSToSphere;
	vSToE = posE - posS;
	vSToSphere = posSphere - posS;
	Vector3 normalSToE = RCVector3::normalize(vSToE);

	//直線と球体座標の最近点までの距離を求める
	//始点から最近点までの距離
	float distance = RCVector3::dot(vSToSphere, normalSToE);
	//始点から終点までの距離に対する、視点から最近点までの距離の比率
	float ratio = distance / RCVector3::length(vSToE);
	//始点から最近点までのベクトル
	Vector3 rangeVec = (normalSToE * distance);

	//posSより手前に球が存在する場合
	if (distance < 0)
	{
		//半径と比較し判定
		if (RCVector3::distance_(posSphere, posS) < sphereRadius)
		{
			colpara.colFlag = true;
			colpara.colPos = posS;
		}
	}
	//posEより奥に球が存在する場合
	else if (distance > 1)
	{
		//半径と比較し判定
		if (RCVector3::distance_(posSphere, posE) < sphereRadius)
		{
			colpara.colFlag = true;
			colpara.colPos = posE;
		}
	}
	if (ratio >= 0 && ratio <= 1)
	{
		//最近点
		Vector3 nearestPoint = posS + rangeVec;

		//球座標と最近点の距離を求める
		float h = RCVector3::distance_(posSphere, nearestPoint);

		//半径と比較し、判定
		if (h <= sphereRadius)
		{
			colpara.colFlag = true;
			colpara.colPos = nearestPoint;
		}
	}

	return colpara;
}

//球と球のあたり判定
CollisionParameter IsCollideSphereToSphere(Vector3 posS, Vector3 posE, float radiusS, float radiusE){
	CollisionParameter colpara;
	
	float distance = RCVector3::distance_(posS, posE);
	float r = radiusS + radiusE;
	colpara.colFlag = false;
	if (distance <= r)colpara.colFlag = true;

	return colpara;
}

//球とカプセルのあたり判定
CollisionParameter IsCollideCapsuleToSphere(Vector3 posS, Vector3 posE, Vector3 posSphere, float sphereRadius, Vector3 nor)
{
	CollisionParameter colpara;
	//当たってない
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);
	Vector3 vSToE, vSToSphere;
	vSToE = posE - posS;
	vSToSphere = posSphere - posS;

	if (RCVector3::length(posE - posSphere) < sphereRadius){
		colpara.colPos = posE;
		colpara.colFlag = true;
		colpara.colNormal = nor;
		return colpara;
	}
	if(RCVector3::length(posS - posSphere) < sphereRadius){
		colpara.colPos = posS;
		colpara.colFlag = true;
		colpara.colNormal = nor;
		return colpara;
	}

	//直線と球体座標の最近点を求める
	Vector3 normalSToE = RCVector3::normalize(vSToE);
	float distance = RCVector3::dot(vSToSphere, normalSToE);
	Vector3 rangeVec = (normalSToE * distance);
	float range = RCVector3::length(rangeVec);
	if (range > RCVector3::length(vSToE) || distance <= 0)return colpara;

	Vector3 nearestPoint = posS + rangeVec;

	//DrawSphere3D(RCConverter::convertVECTOR(&nearestPoint), 200, 100, 10, 10, 1);
	//球体座標と最近点の距離を求める
	float h = RCVector3::distance_(posSphere, nearestPoint);

	//球体半径と比較し、判定
	if (h <= sphereRadius)
	{
		//*resultHitPos = nearestPoint;
		colpara.colFlag = true;
		colpara.colPos = nearestPoint;
		colpara.colNormal = nor;
		return colpara;
	}

	return colpara;
}

//三角形と線分のあたり判定
CollisionParameter IsCollideTriangleLine(Vector3 a, Vector3 b, Vector3 c, Vector3 pos, Vector3 down){
	ModelTriangle tri = { a, b, c };
	Matrix4 mat = RCMatrix4::Identity();
	CollisionParameter colpara = ModelRay(&tri, mat, pos, down);

	return colpara;
}

//三角形と線分のあたり判定
CollisionParameter IsCollideTriangleSphere(Vector3 a, Vector3 b, Vector3 c, Vector3 pos,float radiuw){
	ModelTriangle tri = { a, b, c };
	Matrix4 mat = RCMatrix4::Identity();
	CollisionParameter colpara = ModelSphere(&tri, mat, pos, radiuw);

	return colpara;
}