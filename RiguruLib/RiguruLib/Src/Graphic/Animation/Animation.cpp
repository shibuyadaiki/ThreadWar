#include "Animation.h"
#include "../../Math/Matrix4.h"
#include "../../Math/MatrixUtility.h"
#include "../../Math/Converter.h"

Animation::Animation(){

}
Animation::~Animation(){

}

//�A�j���[�V�����̃��[�h
void Animation::Load(ANIM_ID id, const char* animName){
	rl.Load(animName, &au[id]);
}

//�A�j���[�V�����̃o�C���h
void Animation::BindAnim(Shader* shader, float animCount){
//	Matrix4* m = PopAnim(&au[ANIM_ID::NEPHILA_WAIT_ANIM], animCount);
//	D3DXMATRIX mm[BONEMAXCOUNT];
//	for (int i = 0; i < BONEMAXCOUNT; i++){
//		mm[i] = RConvert(&m[i]);
//	}
//	shader->SetMatrix("boneMatrix", mm, BONEMAXCOUNT);
//	SAFE_DELETE_ARRAY(m);
}

//�A�j���[�V�����̃o�C���h
void Animation::BindAnim(Shader* shader, Matrix4* animMat){
	D3DXMATRIX mm[BONEMAXCOUNT];
	for (int i = 0; i < BONEMAXCOUNT; i++){
		mm[i] = RConvert(&animMat[i]);
	}
	shader->SetMatrix("boneMatrix", mm, BONEMAXCOUNT);
}

//�A�j���[�V�����̃o�C���h(�u�����h)
void Animation::BindAnimBlend(Animation& otherAni, Shader* shader, float animCount, float otherAnimCount, float blendLevel){
	//Matrix4* m = AnimBlend(otherAni, animCount, otherAnimCount, blendLevel);
	//D3DXMATRIX mm[BONEMAXCOUNT];
	//for (int i = 0; i < BONEMAXCOUNT; i++){
	//	mm[i] =RConvert(&m[i]);
	//}
	//shader->SetMatrix("boneMatrix", mm, BONEMAXCOUNT);
	//SAFE_DELETE(m);
}

//�w��t���[���̃A�j���[�V�����𒊏o
void Animation::PopAnim(Matrix4* animMat, AnimUser* animUser, float animCount, bool roop){
	//int�ɃL���X�g���ăN�����v�ƃ}�C�i�X�̏ꍇ���l����
	int animCount_ = (int)abs(animCount);
	//���̃t���[���̃J�E���g(float�Ńt���[���̊Ԃ��w�肷��Ƃ��������ɂȂ邽�߂��̊Ԃ̃u�����h�p)
	int animCountAdd1_ = animCount_ + 1;
	int size = (*animUser->ReturnMat())[0].size() - 1;
	if (size != 0){
		//�N�����v
		animCount_ %= size;
		animCountAdd1_ %= size;
		//�}�C�i�X��������
		if (animCount < 0){
			animCount_ = size - animCount_;
			animCountAdd1_ = size - animCountAdd1_;
		}
	}
	//�|�[�Y�f�[�^�̏ꍇ
	else{
		animCount_ = 0;
		animCountAdd1_ = 0;
	}
	if (!roop){
		if (abs(animCount) > size - 1){
			animCount_ = size;
			animCountAdd1_ = size;
		}
	}
	for (int fdf = 0; fdf < (*animUser->ReturnMat()).size(); fdf++){
		animMat[fdf] = AnimBlend(RConvert(&(*animUser->ReturnMat())[fdf][animCount_]),
			RConvert(&(*animUser->ReturnMat())[fdf][animCountAdd1_]), abs(animCount) - (int)abs(animCount));
	}
}
void Animation::PopAnim(Matrix4* animMat, ANIM_ID id, float animCount, bool roop){
	PopAnim(animMat,&au[id], animCount, roop);
}

//�u�����h
void Animation::AnimBlend(Matrix4* animMat, ANIM_ID id, float animCount, ANIM_ID otherID, float otherAnimCount, float blendLevel, bool roop){	
	Matrix4 m[BONEMAXCOUNT];
	PopAnim(m,&au[id], animCount,roop);
	if (otherID != ANIM_ID::NULL_ANIM){
		Matrix4 mm[BONEMAXCOUNT];
		PopAnim(mm,&au[otherID], otherAnimCount, roop);
		for (int fdf = 0; fdf < (*au[id].ReturnMat()).size(); fdf++){
			animMat[fdf] = RCMatrix4::lerp(
				m[fdf],
				mm[fdf],
				blendLevel);
		}
	}
	else{
		for (int fdf = 0; fdf < (*au[id].ReturnMat()).size(); fdf++){
			animMat[fdf] = m[fdf];
		}
	}
}

void Animation::AnimBlend(Matrix4* mat, Matrix4* otherMat,float blendLevel,ANIM_ID id){
	for (int fdf = 0; fdf < (*au[id].ReturnMat()).size(); fdf++){
		mat[fdf] = RCMatrix4::lerp(
			mat[fdf],
			otherMat[fdf],
			blendLevel);
	}
}

Matrix4 Animation::AnimBlend(Matrix4 mat, Matrix4 otherMat, float blendLevel){
	return RCMatrix4::lerp(
		mat,
		otherMat,
		blendLevel);
}

AnimUser* Animation::ReturnAnimUser(ANIM_ID id){
	return &au[id];
}