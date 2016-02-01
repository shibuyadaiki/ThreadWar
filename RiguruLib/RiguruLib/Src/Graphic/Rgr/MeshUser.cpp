#include "MeshUser.h"

MeshUser::MeshUser(){

}

MeshUser::~MeshUser(){

}

void MeshUser::SetVertex(int childCount, D3DXVECTOR3 vertex){
	vertexVec[childCount].push_back(vertex);
}
void MeshUser::SetIndex(int childCount, int index){
	indexVec[childCount].push_back(index);
}
void MeshUser::SetUV(int childCount, D3DXVECTOR2 uv){
	uvVec[childCount].push_back(uv);
}
void MeshUser::SetNormal(int childCount, D3DXVECTOR3 normal){
	normalVec[childCount].push_back(normal);
}
void MeshUser::SetTex(int childCount,std::string& c){
	texVec[childCount] = c;
}
void MeshUser::SetAmbientColor(int childCount, D3DXVECTOR3 color){
	ambientColor[childCount] = color;
}
void MeshUser::SetDiffuseColor(int childCount, D3DXVECTOR3 color){
	diffuseColor[childCount] = color;
}
void MeshUser::SetSpecularColor(int childCount, D3DXVECTOR3 color){
	specularColor[childCount] = color;
}
void MeshUser::SetEmissiveColor(int childCount, D3DXVECTOR3 color){
	emissiveColor[childCount] = color;
}
void MeshUser::SetNormalMap(int childCount, D3DXVECTOR3 normal){
	normalMap[childCount] = normal;
}
void MeshUser::SetBumpMap(int childCount, D3DXVECTOR3 bump){
	bumpMap[childCount] = bump;
}
void MeshUser::SetShininess(int childCount, float shin){
	shininessColor[childCount] = shin;
}
void MeshUser::SetTransparent(int childCount, float trans){
	transparent[childCount] = trans;
}
void MeshUser::SetReflection(int childCount, D3DXVECTOR3 ref){
	reflection[childCount] = ref;
}
void MeshUser::SetBoneIndex(int childCount, int boneCount, int index){
	boneIndex[childCount][boneCount].push_back(index);
}
void MeshUser::SetWeight(int childCount, int index_, float weight_){
	weight[childCount][index_].push_back(weight_);
}
void MeshUser::SetStartMat(int childCount, int boneCount, D3DXMATRIX startMat_){
	startMat[childCount][boneCount] = startMat_;
}
void MeshUser::SetMat(int childCount, int boneCount, D3DXMATRIX mat_){
	mat[childCount][boneCount].push_back(mat_);
}
void MeshUser::SetAnimFlag(bool f){
	animFlag = f;
}

std::map<int,std::vector<D3DXVECTOR3>>* MeshUser::ReturnVertex(){
	return &vertexVec;
}
std::map<int, std::vector<int>>* MeshUser::ReturnIndex(){
	return &indexVec;
}
std::map<int, std::vector<D3DXVECTOR2>>* MeshUser::ReturnUV(){
	return &uvVec;
}
std::map<int, std::vector<D3DXVECTOR3>>* MeshUser::ReturnNormal(){
	return &normalVec;
}
std::map<int,std::string>* MeshUser::ReturnTex(){
	return &texVec;
}
std::map<int, D3DXVECTOR3>* MeshUser::ReturnAmbientColor(){
	return &ambientColor;
}
std::map<int, D3DXVECTOR3>* MeshUser::ReturnDiffuseColor(){
	return  &diffuseColor;
}
std::map<int, D3DXVECTOR3>* MeshUser::ReturnSpecularColor(){
	return &specularColor;
}
std::map<int, D3DXVECTOR3>* MeshUser::ReturnEmissiveColor(){
	return &emissiveColor;
}
std::map<int, D3DXVECTOR3>* MeshUser::ReturnNormalMap(){
	return &normalMap;
}
std::map<int, D3DXVECTOR3>* MeshUser::ReturnBumpMap(){
	return &bumpMap;
}
std::map<int, float>* MeshUser::ReturnShininess(){
	return &shininessColor;
}
std::map<int, float>* MeshUser::ReturnTransparent(){
	return &transparent;
}
std::map<int, D3DXVECTOR3>* MeshUser::ReturnReflection(){
	return &reflection;
}
std::map<int, std::map<int, std::vector<int>>>* MeshUser::ReturnBoneIndex(){
	return &boneIndex;
}
std::map<int, std::map<int,std::vector<float>>>* MeshUser::ReturnWeight(){
	return &weight;
}
std::map<int, std::map<int, D3DXMATRIX>>* MeshUser::ReturnStartMat(){
	return &startMat;
}
std::map<int, std::map<int, std::vector<D3DXMATRIX>>>* MeshUser::ReturnMat(){
	return &mat;
}
bool MeshUser::ReturnAnimFlag(){
	return animFlag;
}