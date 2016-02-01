#include "AnimUser.h"

AnimUser::AnimUser(){

}

AnimUser::~AnimUser(){

}

void AnimUser::SetMat(int boneCount, D3DXMATRIX mat_){
	mat[boneCount].push_back(mat_);
}

void AnimUser::SetAnimFlag(bool f){
	animFlag = f;
}

std::map<int, std::vector<D3DXMATRIX>>* AnimUser::ReturnMat(){
	return &mat;
}

bool AnimUser::ReturnAnimFlag(){
	return animFlag;
}