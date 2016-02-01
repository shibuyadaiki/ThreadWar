#pragma once

#include "../../Other/DX11User.h"
#include <vector>
#include <map>

class MeshUser{
public:
	MeshUser();
	~MeshUser();
	void SetVertex(int childCount,D3DXVECTOR3 vertex);
	void SetIndex(int childCount, int index);
	void SetUV(int childCount, D3DXVECTOR2 uv);
	void SetNormal(int childCount, D3DXVECTOR3 normal);
	void SetTex(int childCount, std::string& c);
	void SetAmbientColor(int childCount, D3DXVECTOR3 color);
	void SetDiffuseColor(int childCount, D3DXVECTOR3 color);
	void SetSpecularColor(int childCount, D3DXVECTOR3 color);
	void SetEmissiveColor(int childCount, D3DXVECTOR3 color);
	void SetNormalMap(int childCount, D3DXVECTOR3 normal);
	void SetBumpMap(int childCount, D3DXVECTOR3 bump);
	void SetShininess(int childCount, float shin);
	void SetTransparent(int childCount, float trans);
	void SetReflection(int childCount, D3DXVECTOR3 ref);
	void SetBoneIndex(int childCount, int index, int boneIndex);
	void SetWeight(int childCount, int index,float weight_);
	void SetStartMat(int childCount, int boneCount, D3DXMATRIX startMat);
	void SetMat(int childCount, int boneCount, D3DXMATRIX mat);
	void SetAnimFlag(bool f);

	std::map<int, std::vector<D3DXVECTOR3>>* ReturnVertex();
	std::map<int, std::vector<int>>* ReturnIndex();
	std::map<int, std::vector<D3DXVECTOR2>>* ReturnUV();
	std::map<int, std::vector<D3DXVECTOR3>>* ReturnNormal();
	std::map<int, std::string>* ReturnTex();
	std::map<int, D3DXVECTOR3>* ReturnAmbientColor();
	std::map<int, D3DXVECTOR3>* ReturnDiffuseColor();
	std::map<int, D3DXVECTOR3>* ReturnSpecularColor();
	std::map<int, D3DXVECTOR3>* ReturnEmissiveColor();
	std::map<int, D3DXVECTOR3>* ReturnNormalMap();
	std::map<int, D3DXVECTOR3>* ReturnBumpMap();
	std::map<int, float>* ReturnShininess();
	std::map<int, float>* ReturnTransparent();
	std::map<int, D3DXVECTOR3>* ReturnReflection();
	std::map<int, std::map<int, std::vector<int>>>* ReturnBoneIndex();
	std::map<int, std::map<int, std::vector<float>>>* ReturnWeight();
	std::map<int, std::map<int, D3DXMATRIX>>* ReturnStartMat();
	std::map<int, std::map<int, std::vector<D3DXMATRIX>>>* ReturnMat();
	bool ReturnAnimFlag();
private:
	std::map<int,std::vector<D3DXVECTOR3>> vertexVec;
	std::map<int, std::vector<int>> indexVec;
	std::map<int, std::vector<D3DXVECTOR2>> uvVec;
	std::map<int, std::vector<D3DXVECTOR3>> normalVec;
	std::map<int, std::string> texVec;

	//ライト
	//アンビエント
	std::map < int, D3DXVECTOR3> ambientColor;
	//ディフューズ
	std::map<int, D3DXVECTOR3> diffuseColor;
	//スペキュラ
	std::map<int, D3DXVECTOR3> specularColor;
	//エミッシブ
	std::map<int, D3DXVECTOR3> emissiveColor;
	//ノーマルマップ
	std::map<int, D3DXVECTOR3> normalMap;
	//バンプマップ
	std::map<int, D3DXVECTOR3> bumpMap;
	//シャイネス
	std::map<int, float> shininessColor;
	//リフレクション(反射)
	std::map<int, D3DXVECTOR3> reflection;
	//トランスペアレント(透過度)
	std::map<int, float> transparent;
	//インデックス
	std::map<int, std::map<int, std::vector<int>>> boneIndex;
	//ウェイト
	std::map<int, std::map<int,std::vector<float>>>weight;


	//初期マトリックス
	std::map<int, std::map<int, D3DXMATRIX>> startMat;
	//マトリックス
	std::map<int, std::map<int, std::vector<D3DXMATRIX>>> mat;

	bool animFlag;
};