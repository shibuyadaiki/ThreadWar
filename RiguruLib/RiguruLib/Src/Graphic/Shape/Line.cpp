#include "Line.h"
#include "../../Other/Device.h"
#include <iostream>
#include <fstream>
#include "../Rgr/RgrLoader.h"
#include "../../Math/MathUtility.h"
#include "../../Math/Converter.h"
using namespace std;


Line::Line(){
}

Line::~Line(){
}

void Line::Release(){
	SAFE_RELEASE(g_pIndexBufferList[0]);
	SAFE_RELEASE(g_pVertexBufferList[0]);
}

HRESULT Line::Load(){
	HRESULT hr = E_FAIL;
	std::vector<char> fileNameVec;
	char slash = '\\';

	std::vector<VERTEX> vertexVec;

	vertexVec.push_back({
		(D3DXVECTOR3(0,0,0)),
		(D3DXVECTOR3(0, 0, 0)),
		(D3DXVECTOR2(1, 1)),
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	});
	vertexVec.push_back({
		(D3DXVECTOR3(0, 1,0)),
		(D3DXVECTOR3(0, 0, 0)),
		(D3DXVECTOR2(0, 1)),
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	});

	int vertexSize = vertexVec.size();
	// ���_�̎��f�[�^��ݒ�
	VERTEX* v = new VERTEX[vertexSize];
	for (int i = 0; i < vertexSize; i++)
		v[i] = vertexVec[i];

	// ���_�o�b�t�@
	ID3D11Buffer* VertexBuffer = NULL;

	// ���_�o�b�t�@���쐬����
	Device::GetInstance().Getd3d11User()->CreateVertexBuffer(&VertexBuffer, v, sizeof(VERTEX)* vertexSize, 0);
	g_pVertexBufferList.push_back(VertexBuffer);
	delete(v);

	UINT* index = new UINT[2];

	index[0] = 0;
	index[1] = 1;

	// �C���f�b�N�X�o�b�t�@
	ID3D11Buffer* IndexBuffer = NULL;

	// �C���f�b�N�X�o�b�t�@���쐬����B
	hr = Device::GetInstance().Getd3d11User()->CreateIndexBuffer(&IndexBuffer, index, sizeof(UINT)* 2, 0);
	g_pIndexBufferList.push_back(IndexBuffer);
	if (FAILED(hr)) goto EXIT;
	delete(index);
	return S_OK;
EXIT:
	return hr;
}
void Line::Draw(Shader* shader, const Vector3& startPos, const Vector3& endPos, CAMERA_ID cID, D3DXCOLOR& color){
	Matrix4 ide = RCMatrix4::Identity();
	D3DXMATRIX matWorld = RConvert(&ide);
	// �s����G�t�F�N�g�ɐݒ�
	shader->SetMatrix("g_matW", &matWorld);
	shader->SetMatrix("g_matV", &Device::GetInstance().GetCamera(cID)->returnView());
	shader->SetMatrix("g_matP", &Device::GetInstance().GetCamera(cID)->returnProj());
	shader->SetVector4("startPos", &D3DXVECTOR4(startPos.x, startPos.y, startPos.z,1));
	shader->SetVector4("endPos", &D3DXVECTOR4(endPos.x, endPos.y, endPos.z, 1));
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	//Device::GetInstance().Getd3d11User()->SetBlendState(&Device::GetInstance().Getd3d11User()->GetAlignmentBlendDesc(), 1, FALSE);
	Device::GetInstance().Getd3d11User()->SetTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	
	Device::GetInstance().Getd3d11User()->SetVertexBuffer(&g_pVertexBufferList[0], &stride, &offset);
	Device::GetInstance().Getd3d11User()->SetIndexBuffer(g_pIndexBufferList[0]);
	shader->SetColor("materialColor", &color);

	// �v���~�e�B�u �^�C�v����уf�[�^�̏����Ɋւ������ݒ�
	//Z�̎���STRIP
	shader->ShaderApply();
	Device::GetInstance().Getd3d11User()->DrawIndexed(2, 0, 0);
}