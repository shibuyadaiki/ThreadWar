#include "OctreeLoader.h"
#include<fstream>
#include<iostream>
#include<string>
#include<sstream> 
#include <vector>

using namespace std;

OctreeLoader::OctreeLoader(){

}

OctreeLoader::~OctreeLoader(){
}

//octƒtƒ@ƒCƒ‹‚ğ“Ç‚İ‚İOctreeUser‚É•Û‘¶‚·‚é
HRESULT OctreeLoader::Load(const char* modelName, OctreeUser* o){
	FILE* fp;
	fopen_s(&fp, modelName, "rb");

	int size;
	fread(&size, sizeof(int), 1, fp);
	for (int i = 0; i < size; i++){
		int cubeBunkatuSize;
		fread(&cubeBunkatuSize, sizeof(int), 1, fp);
		CubeParameter* c = new CubeParameter[cubeBunkatuSize];
		fread(c, sizeof(CubeParameter), cubeBunkatuSize, fp);
		for (int a = 0; a < cubeBunkatuSize; a++){
			o->SetCubeVec(i,c[a]);
		}
		delete[]c;
	}

	fread(&size, sizeof(int), 1, fp);
	for (int i = 0; i < size; i++){
		int triSize;
		fread(&triSize, sizeof(int), 1, fp);
		int first;
		fread(&first, sizeof(int), 1, fp);
		for (int ia = 0; ia < triSize; ia++){
			int mesh;
			fread(&mesh, sizeof(int), 1, fp);
			int secondSize;
			fread(&secondSize, sizeof(int), 1, fp);
			ModelTriangle* mt = new ModelTriangle[secondSize];
			fread(mt, sizeof(ModelTriangle), secondSize, fp);
			for (int a = 0; a < secondSize; a++){
				o->SetTriangleInthirdCubeVec(first, mesh, mt[a]);
			}

			SAFE_DELETE(mt);
		}
	}
	fclose(fp);
	return S_OK;
}