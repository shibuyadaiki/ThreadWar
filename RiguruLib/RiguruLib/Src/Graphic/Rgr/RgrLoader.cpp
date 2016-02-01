#include "RgrLoader.h"
#include<fstream>
#include<iostream>
#include<string>
#include<sstream> 
#include <vector>

using namespace std;

RgrLoader::RgrLoader(){

}

RgrLoader::~RgrLoader(){
	for (int i = 0; i < childCount; i++){
		texName[i].clear();
	}
}

vector<string> split(const string &str, char delim){
	vector<string> res;
	size_t current = 0, found;
	while ((found = str.find_first_of(delim, current)) != string::npos){
		res.push_back(string(str, current, found - current));
		current = found + 1;
	}
	res.push_back(string(str, current, str.size() - current));
	return res;
}

HRESULT RgrLoader::Load(const char* modelName, MeshUser* m){
	std::ifstream model(modelName);
	std::string column;

	childCount = 0;

	FILE* fp;
	fopen_s(&fp, modelName, "rb");

	char* columnChar = new char[256];
	int modelSize;
	fread(&modelSize, sizeof(int), 1, fp);
	for (int mod = 0; mod < modelSize; mod++){
		struct Vertex
		{
			float vx;
			float vy;
			float vz;
		};
		int vertexSize;
		fread(&vertexSize, sizeof(int), 1, fp);
		Vertex* v = new Vertex[vertexSize];
		fread(v, sizeof(Vertex), vertexSize, fp);
		for (int i = 0; i < vertexSize; i++){
			D3DXVECTOR3 vertex(v[i].vx, v[i].vy, v[i].vz);
			m->SetVertex(mod, vertex);
		}
		delete[]v;

		int indexSize;
		fread(&indexSize, sizeof(int), 1, fp);
		int* ind = new int[indexSize];
		fread(ind, sizeof(int), indexSize, fp);
		for (int i = 0; i < indexSize; i++){
			m->SetIndex(mod, ind[i]);
		}
		delete[]ind;

		int normalSize;
		fread(&normalSize, sizeof(int), 1, fp);
		Vertex* n = new Vertex[normalSize];
		fread(n, sizeof(Vertex), normalSize, fp);
		for (int i = 0; i < normalSize; i++){
			D3DXVECTOR3 vertex(n[i].vx, n[i].vy, n[i].vz);
			m->SetNormal(mod, vertex);
		}
		delete[]n;

		struct UV
		{
			float vx;
			float vy;
		};
		int uvSize;
		fread(&uvSize, sizeof(int), 1, fp);
		UV* uv = new UV[uvSize];
		fread(uv, sizeof(UV), uvSize, fp);
		for (int i = 0; i < uvSize; i++){
			D3DXVECTOR2 vertex(uv[i].vx, uv[i].vy);
			m->SetUV(mod, vertex);
		}
		delete[]uv;

		int textureSize;
		fread(&textureSize, sizeof(int), 1, fp);
		char* texture = new char[textureSize + 1];
		texture[textureSize] = '\0';
		fread(texture, sizeof(const char), textureSize, fp);
		string s = texture;
		m->SetTex(mod, s);
		delete[] texture;

		Vertex amb;
		fread(&amb, sizeof(Vertex), 1, fp);
		D3DXVECTOR3 ambient(amb.vx, amb.vy, amb.vz);
		m->SetAmbientColor(mod, ambient);

		Vertex dif;
		fread(&dif, sizeof(Vertex), 1, fp);
		D3DXVECTOR3 diffuse(dif.vx, dif.vy, dif.vz);
		m->SetDiffuseColor(mod, diffuse);

		Vertex spe;
		fread(&spe, sizeof(Vertex), 1, fp);
		D3DXVECTOR3 specular(spe.vx, spe.vy, spe.vz);
		m->SetAmbientColor(mod, specular);

		Vertex emi;
		fread(&emi, sizeof(Vertex), 1, fp);
		D3DXVECTOR3 emissive(emi.vx, emi.vy, emi.vz);
		m->SetEmissiveColor(mod, emissive);

		Vertex normap;
		fread(&normap, sizeof(Vertex), 1, fp);
		D3DXVECTOR3 normalMap(normap.vx, normap.vy, normap.vz);
		m->SetNormalMap(mod, normalMap);

		Vertex bummap;
		fread(&bummap, sizeof(Vertex), 1, fp);
		D3DXVECTOR3 bumpMap(bummap.vx, bummap.vy, bummap.vz);
		m->SetBumpMap(mod, bumpMap);

		float tran;
		fread(&tran, sizeof(float), 1, fp);
		m->SetTransparent(mod, tran);

		int boneSize;
		fread(&boneSize, sizeof(int), 1, fp);

		for (int bone = 0; bone < boneSize; bone++){
			int childBone;
			fread(&childBone, sizeof(int), 1, fp);
			int* bInd = new int[childBone];
			fread(bInd, sizeof(int), childBone, fp);

			for (int i = 0; i < childBone; i++){
				m->SetBoneIndex(mod, bone, bInd[i]);
			}
			delete[]bInd;
			for (int i = 0; i < 4 - childBone; i++){
				m->SetBoneIndex(mod, bone, 0);
			}
			float* bWei = new float[childBone];
			fread(bWei, sizeof(float), childBone, fp);
			for (int i = 0; i < childBone; i++){
				m->SetWeight(mod, bone, bWei[i]);
			}
			for (int i = 0; i < 4 - childBone; i++){
				m->SetWeight(mod, bone, 0);
			}
			delete[]bWei;

			//int boneSize;
			//fread(&boneSize, sizeof(int), 1, fp);
			//
			//for (int fd = 0; fd < boneSize; fd++){
			//	int boneS;
			//	fread(&boneS, sizeof(int), 1, fp);
			//	int* bBone = new int[boneS];
			//	fread(bBone, sizeof(int), boneS, fp);	
			//	for (int fds = 0; fds < boneS;fds++)
			//			//		m->SetBone(mod, bInd[fd], bBone[fds]);
			//	delete[]bBone;
			//}
		}
	}
	delete[] columnChar;
	fclose(fp);
	return S_OK;
}