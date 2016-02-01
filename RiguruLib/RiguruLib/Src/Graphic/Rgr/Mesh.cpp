#include "Mesh.h"
#include "../../Other/Device.h"
#include <iostream>
#include <fstream>
#include "RgrLoader.h"
#include "../../Math/MathUtility.h"

using namespace std;


Mesh::Mesh(){
	ani.SetAnimFlag(false);
}

Mesh::~Mesh(){

}

void Mesh::Release(){
	for (int i = 0; i < meshCount + 1; i++){
		SAFE_RELEASE(g_pIndexBufferList[i]);
		SAFE_RELEASE(g_pVertexBufferList[i]);
		SAFE_RELEASE(tex[i]);
	}
}

HRESULT Mesh::Load( const char* modelName){
	rL.Load(modelName, &m);
	HRESULT hr = E_FAIL;
	meshCount = (*m.ReturnIndex()).size() - 1;
	for (int meshCountFor = 0; meshCountFor < meshCount + 1; meshCountFor++){
		std::vector<VERTEX> vertexVec;
		indexSize.push_back((*m.ReturnIndex())[meshCountFor].size());
		std::vector<UINT> index(indexSize[meshCountFor]);

		for (int indexSizeFor = 0; indexSizeFor < indexSize[meshCountFor]; indexSizeFor++){
			//for (int indexSizeFor = indexSize[meshCountFor] - 1; indexSizeFor >= 0; indexSizeFor--){
			index[indexSizeFor] = indexSizeFor;
			int idx = (*m.ReturnIndex())[meshCountFor][indexSizeFor];
			vertexVec.push_back({ (*m.ReturnVertex())[meshCountFor][idx],
				(*m.ReturnNormal())[meshCountFor][(*m.ReturnNormal())[meshCountFor].size() == (*m.ReturnVertex())[meshCountFor].size() ? idx : indexSizeFor],
				(*m.ReturnUV())[meshCountFor][indexSizeFor],
				{ (*m.ReturnBoneIndex())[meshCountFor][idx][0], (*m.ReturnBoneIndex())[meshCountFor][idx][1], (*m.ReturnBoneIndex())[meshCountFor][idx][2], (*m.ReturnBoneIndex())[meshCountFor][idx][3] },
				{ (*m.ReturnWeight())[meshCountFor][idx][0] * 100, (*m.ReturnWeight())[meshCountFor][idx][1] * 100, (*m.ReturnWeight())[meshCountFor][idx][2] * 100, (*m.ReturnWeight())[meshCountFor][idx][3] * 100 }
			});
		}

		int vertexSize = vertexVec.size();
		// 頂点の実データを設定
		VERTEX* v = new VERTEX[vertexSize];
		for (int i = 0; i < vertexSize; i++)
			v[i] = vertexVec[i];

		// 頂点バッファ
		ID3D11Buffer* VertexBuffer = NULL;
		// インデックスバッファ
		ID3D11Buffer* IndexBuffer = NULL;

		// インデックスバッファを作成する。
		hr = Device::GetInstance().Getd3d11User()->CreateIndexBuffer(&IndexBuffer, &index[0], sizeof(UINT)* indexSize[meshCountFor], 0);
		g_pIndexBufferList.push_back(IndexBuffer);
		if (FAILED(hr)) goto EXIT;

		// 頂点バッファを作成する
		hr = Device::GetInstance().Getd3d11User()->CreateVertexBuffer(&VertexBuffer, v, sizeof(VERTEX)* vertexSize, 0);
		g_pVertexBufferList.push_back(VertexBuffer);
		if (FAILED(hr)) goto EXIT;
		delete[](v);
		std::vector<char> fileNameVec;
		char slash = '\\';

		for (int i = 0; i < strlen(modelName); i++)
			fileNameVec.push_back(modelName[i]);

		fileNameVec.push_back('/');
		fileNameVec.push_back('.');
		fileNameVec.push_back('.');
		fileNameVec.push_back('/');
		std::string nullCheck = (*m.ReturnTex())[meshCountFor];
		//nullCheck ="NULL";
		if (nullCheck != "NULL"){
			for (int i = 0; i < (*m.ReturnTex())[meshCountFor].size(); i++)
				fileNameVec.push_back((*m.ReturnTex())[meshCountFor][i]);
			std::string fileName;
			for (int i = 0; i < fileNameVec.size(); i++){
				fileName += fileNameVec[i];
			}
			tex.push_back(lt.LoadBindTexture(fileName.c_str()));
		}
		else{
			tex.push_back(lt.LoadBindTexture(NULL));
		}
	}
	return S_OK;
EXIT:
	return hr;
}

float count2 = 0;
bool playFlag = false;
bool stopFlag = false;
void Mesh::Draw(D3DXMATRIX* matWorld, bool animFlag, Shader* shader, CAMERA_ID cID, D3DXCOLOR* color, bool alphaFlag){
	D3DXVECTOR4 g_vecLight = D3DXVECTOR4(-0.5f, -0.5f, 0.5f, 0);
	// 行列をエフェクトに設定
	shader->SetMatrix("g_matW", matWorld);
	shader->SetMatrix("g_matV", &Device::GetInstance().GetCamera(cID)->returnView());
	shader->SetMatrix("g_matP", &Device::GetInstance().GetCamera(cID)->returnProj());
	
	shader->SetVector4("g_vecLight", &g_vecLight);

	//shader->SetMatrix("boneMatrix", mat);
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Device::GetInstance().Getd3d11User()->SetAlphaBlend(alphaFlag);
	Device::GetInstance().Getd3d11User()->SetTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (int i = 0; i < meshCount + 1; i++){

		Device::GetInstance().Getd3d11User()->SetVertexBuffer(&g_pVertexBufferList[i], &stride, &offset);

		Device::GetInstance().Getd3d11User()->SetIndexBuffer(g_pIndexBufferList[i]);
		
		// インデックスバッファ設定
		shader->SetTexture("g_Tex", tex[i]);

		D3DXCOLOR* materialColor;
		if (color == NULL){
			materialColor = &D3DXCOLOR(
				(*m.ReturnDiffuseColor())[i].x,
				(*m.ReturnDiffuseColor())[i].y,
				(*m.ReturnDiffuseColor())[i].z,
				(*m.ReturnTransparent())[i]);
		}
		else materialColor = color;

		shader->SetColor("materialColor", materialColor);

		// プリミティブ タイプおよびデータの順序に関する情報を設定
		//Zの時はSTRIP
		shader->ShaderApply();
		Device::GetInstance().Getd3d11User()->DrawIndexed(indexSize[i], 0, 0);
	}
	Device::GetInstance().Getd3d11User()->SetAlphaBlend(FALSE);
}

MeshUser* Mesh::ReturnMeshUser(){
	return &m;
}