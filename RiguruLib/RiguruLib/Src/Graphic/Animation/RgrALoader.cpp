#include "RgrALoader.h"
#include<fstream>
#include<iostream>
#include<string>
#include<sstream> 
#include <vector>

using namespace std;

RgrALoader::RgrALoader(){

}

RgrALoader::~RgrALoader(){
}

HRESULT RgrALoader::Load(const char* modelName, AnimUser* ani){
	std::ifstream model(modelName);
	std::string column;

	
	FILE* fp;
	fopen_s(&fp, modelName, "rb");

	int matSize;
	fread(&matSize, sizeof(int), 1, fp);
	for (int as = 0; as < matSize; as++){
		int animSize;
		fread(&animSize, sizeof(int), 1, fp);
		D3DXMATRIX* matVec = new D3DXMATRIX[animSize];
		fread(matVec, sizeof(D3DXMATRIX), animSize, fp);
		for (int i = 0; i < animSize; i++){
			ani->SetMat(as, matVec[i]);
		}
		delete[]matVec;
	}

	bool animFlag;
	fread(&animFlag, sizeof(bool), 1, fp);
	ani->SetAnimFlag(animFlag);
	
	fclose(fp);
	return S_OK;
}