#include "Camera.h"
#include "../../Math/Converter.h"
#include "../../Other/Device.h"
#include ".././Collision.h"
#include "../../Graphic/Graphic.h"
#include "../Stage.h"
const float CAMERA_ANGLE_SPEED = 0.003f * 60.0f;		// ���񑬓x
const float CAMERA_PLAYER_TARGET_HEIGHT = 0.75f;	// �v���C���[���W����ǂꂾ�������ʒu�𒍎��_�Ƃ��邩
const float CAMERA_PLAYER_LENGTH = 2.5f;	// �v���C���[�Ƃ̋���
const float CAMERA_COLLISION_SIZE = 0.2f;	// �J�����̓����蔻��T�C�Y

Camera::Camera(CAMERA_ID cID_, std::weak_ptr<Stage> stage_) :cID(cID_),stage(stage_){
	Initialize();
}
Camera::~Camera(){

}

void Camera::Initialize(){
	isRespawn = false;
	mCameraParam.AngleH = 0;
	mCameraParam.AngleV = Math::radian(-0);
	mCameraParam.Eye = vector3(0, 0, 0);
	mCameraParam.Target = vector3(0, 0, 0);
	mCameraParam.Up = vector3(0, 1, 0);
	mat = RCMatrix4::Identity();
	angleC = 0;
	angleF = 0;
	matrix = RCMatrix4::Identity();
	if (cID == CAMERA_ID::PLAYER_CAMERA_1P ||
		cID == CAMERA_ID::PLAYER_CAMERA_2P ||
		cID == CAMERA_ID::PLAYER_CAMERA_3P ||
		cID == CAMERA_ID::PLAYER_CAMERA_4P)
		matrix = RCMatrix4::rotateY(180.0f);
	cameraMove = true;
	
	siyaChange = 0.0f;
	playerAI = false;
	dokusai = false;
	godPos = vector3(0, 0, 0);
	godFront = 0;
	godLeft = 0;
}
//�|�W�V�����ƃr���[���Z�b�g
void Camera::SetCamera(Vector3 cameraPos, Vector3 cameraView, float frameTime){
	// �ˉe�s��
	float siya = 4.0f;
	float sp = CAMERA_ANGLE_SPEED;
	if (stage._Get()->ReturnStartTime() < 0){
		if (Device::GetInstance().GetInput()->GamePadButtonDown(padNum, GAMEPADKEY::BUTTON_L1) || Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_TAB)){
			sp = CAMERA_ANGLE_SPEED / 5.0f;
			siyaChange += 24.0f * frameTime;
			siyaChange = min(4.0f, siyaChange);
		}
		else{
			siyaChange -= 24.0f * frameTime;
			siyaChange = max(0.0f, siyaChange);
		}
		siya += siyaChange;
		if ((cID == CAMERA_ID::PLAYER_CAMERA_1P || dokusai) && !playerAI)
		{
			mCameraParam.InputAngle = Device::GetInstance().GetInput()->RightStick(padNum) / 500.0f;
			if (mCameraParam.InputAngle == vector3(0, 0, 0)){
				mCameraParam.InputAngle = Device::GetInstance().GetInput()->MouseVec();
				sp = CAMERA_ANGLE_SPEED;
			}
			if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_UP))
				mCameraParam.InputAngle.y = 1.0f;
			if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_DOWN))
				mCameraParam.InputAngle.y = -1.0f;
			if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_RIGHT))
				mCameraParam.InputAngle.x = 1.0f;
			if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_LEFT))
				mCameraParam.InputAngle.x = -1.0f;
		}
	}
	D3DXMatrixPerspectiveFovLH(&matProj, 3.1415926f / siya, 16.0f / 9.0f /*4.0f / 3.0f*/, 0.1f, 10000.0f);
	//if (cID != CAMERA_ID::PLAYER_CAMERA){
	//	Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 485), vector2(0.20f, 0.25f), 0.5f, "AngleH:" + std::to_string(Math::angle(mCameraParam.AngleH)) + "f");
	//	Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 500), vector2(0.20f, 0.25f), 0.5f, "AngleV:" + std::to_string(Math::angle(mCameraParam.AngleV)) + "f");
	//}
	if (!cameraMove || isRespawn){
		mCameraParam.InputAngle = vector3(0, 0, 0);
	}
	//���͒l�����ƂɌv�Z
	mCameraParam.AngleH += mCameraParam.InputAngle.x * sp * frameTime;
	mCameraParam.AngleV += mCameraParam.InputAngle.y * sp * frameTime;
	//�N�����v
	if (mCameraParam.AngleH > Math::radian(180))mCameraParam.AngleH = Math::radian(-180);
	else if (mCameraParam.AngleH < Math::radian(-180))mCameraParam.AngleH = Math::radian(180);
	if (mCameraParam.AngleV > Math::radian(180))mCameraParam.AngleV = Math::radian(-180);
	else if (mCameraParam.AngleV < Math::radian(-180))mCameraParam.AngleV = Math::radian(180);

	//�e�X�g�t�H���g
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 485), vector2(0.20f, 0.25f), 0.5f, "AngleH:" + std::to_string(Math::angle(mCameraParam.AngleH)) + "f");
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 500), vector2(0.20f, 0.25f), 0.5f, "AngleV:" + std::to_string(Math::angle(mCameraParam.AngleV)) + "f");
	//�^�[�Q�b�g���v���C���[��菭����ɃZ�b�g
	mCameraParam.Target = RCMatrix4::getPosition(mat) + cameraPos + RCVector3::normalize(RCMatrix4::getUp(mat)) * CAMERA_PLAYER_TARGET_HEIGHT;//* Time::DeltaTime;

	//�^�[�Q�b�g�̈ʒu�̃}�g���b�N�X�擾
	Matrix4 t = RCMatrix4::translate(mCameraParam.Target);
	//�v���C���[�̍����g��
	Vector3 left = RCVector3::normalize(RCMatrix4::getLeft(mat));
	//�������ɏ㉺�̉�]�����߂�
	mCameraParam.AngleVSub = Math::angle(mCameraParam.AngleV - angleC);
	Matrix4 pitchMat = RCQuaternion::rotate(left, mCameraParam.AngleVSub);
	//�����p(Player.h�Q��)
	angleC = mCameraParam.AngleV;
	//���̑O�ɂ������̉�]���|����
	Vector3 front = RCVector3::normalize(RCMatrix4::getFront(matrix) * pitchMat);
	//�O�ς�p���ď�����߂�
	Vector3 up = RCVector3::normalize(RCVector3::cross(front, left));
	//������ɍ��E�̉�]�����߂�
	mCameraParam.AngleHSub = Math::angle(mCameraParam.AngleH - angleF);
	Matrix4 yawMat = RCQuaternion::rotate(up, mCameraParam.AngleHSub);
	//�����p(Player.h�Q��)
	angleF = mCameraParam.AngleH;
	//�������O�ƍ������߂�
	front = RCVector3::normalize(RCVector3::cross(left, up));// *yawMat);
	//left = left * yawMat;

	//���ɉ�����
	Matrix4 trans = RCMatrix4::translate(front * CAMERA_PLAYER_LENGTH);
	//Target�̈ʒu������ɉ�����
	Matrix4 x = t* trans;
	//�v���C���[�̂������ƃJ���������̂Ȃ��p
	float angle = Math::acos(RCVector3::dot(RCVector3::normalize(RCMatrix4::getFront(mat)), front));
	//��]�s����Z�b�g
	Matrix4 rotate = RCMatrix4::Identity();
	RCMatrix4::setLeft(rotate, left);
	RCMatrix4::setUp(rotate, up);
	RCMatrix4::setFront(rotate, front);
	matrix = rotate;

	//�v���C���[�̂������ƃJ���������̂Ȃ��p��70�x�ȏ�Ȃ�
	if (angle > 70.0f){
		//�P�x�������ċ��߂�
		if (RCVector3::cross(RCVector3::normalize(RCMatrix4::getFront(mat)), front) == left){
			mCameraParam.AngleV -= Math::radian(abs(angle) - 70.0f);
		}
		else{
			mCameraParam.AngleV += Math::radian(abs(angle) - 70.0f);
		}

		pitchMat = RCQuaternion::rotate(left, Math::angle(mCameraParam.AngleV - angleC));
		angleC = mCameraParam.AngleV;
		front = RCVector3::normalize(RCMatrix4::getFront(matrix) * pitchMat);
		up = RCVector3::normalize(RCVector3::cross(front, left));
		front = RCVector3::normalize(RCVector3::cross(left, up));
		angle = Math::acos(RCVector3::dot(RCVector3::normalize(RCMatrix4::getFront(mat)), front));
		rotate = RCMatrix4::Identity();
		RCMatrix4::setLeft(rotate, left);
		RCMatrix4::setUp(rotate, up);
		RCMatrix4::setFront(rotate, front);

		matrix = rotate;
	}

	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 470), vector2(0.20f, 0.25f), 0.5f, "angle:" + std::to_string(angle) + "f");
	trans = RCMatrix4::translate(front * CAMERA_PLAYER_LENGTH);
	x = t* trans;
	mCameraParam.Eye = RCMatrix4::getPosition(x);//* Time::DeltaTime;

	//�J�����Ƃ̂����蔻��(�Q���T���@��p���ċ�����Z�����Ă���)
	bool hitNum = ModelCapsule(*stage._Get()->ReturnMat(), OCT_ID::STAGE_OCT, CreateCapsule(mCameraParam.Eye, mCameraParam.Target, CAMERA_COLLISION_SIZE)).colFlag;
	std::string s = (hitNum == true ? "TRUE" : "FALSE");
	//Graphic::GetInstance().
	(FONT_ID::TEST_FONT, vector2(0, 455), vector2(0.20f, 0.25f), 0.5f, "MODELCAPSULEFLAG:" + s);
	if (hitNum){
		float notHitLength = 0.0f;
		float cameraLen = CAMERA_PLAYER_LENGTH;
		do{
			float testLen = notHitLength + (cameraLen - notHitLength) / 2.0f;
			trans = RCMatrix4::translate(front * testLen);
			x = t* trans;
			mCameraParam.Eye = RCMatrix4::getPosition(x);//* Time::DeltaTime;
			bool hit = ModelCapsule(*stage._Get()->ReturnMat(), OCT_ID::STAGE_OCT, CreateCapsule(mCameraParam.Eye, mCameraParam.Target, CAMERA_COLLISION_SIZE)).colFlag;
			if (hit){
				cameraLen = testLen;
			}
			else{
				notHitLength = testLen;
			}
		} while (cameraLen - notHitLength > 0.01f);
	}

	mCameraParam.Up = up;//* Time::DeltaTime;
	// �r���[�s��
	D3DXMatrixLookAtLH(&matView,
		&RConvert(&mCameraParam.Eye),
		&RConvert(&mCameraParam.Target),
		&RConvert(&mCameraParam.Up));
}


void Camera::GotCamera(float frameTime){
	{
		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_1,true))
			speed += -1;
		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_2,true))
			speed += 1;
		godFront = 0;
		godLeft = 0;
		godUp = 0;
		mCameraParam.InputAngle = Device::GetInstance().GetInput()->RightStick(0) / 500.0f;
		if (mCameraParam.InputAngle == vector3(0, 0, 0)){
			mCameraParam.InputAngle = Device::GetInstance().GetInput()->MouseVec();
		}

		godFront = RCVector3::normalize(Device::GetInstance().GetInput()->LeftStick(0)).y / 500.0f;
		godLeft = RCVector3::normalize(Device::GetInstance().GetInput()->LeftStick(0)).x / 500.0f;
		if (godFront < 0.5f || godLeft < 0.5f){
			if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_UP))
				godFront = 1;
			if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_DOWN))
				godFront = -1;
			if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_RIGHT))
				godLeft = 1;
			if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_LEFT))
				godLeft = -1;
			if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_N))
				godUp = 1;
			if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_M))
				godUp = -1;
		}
	}
	D3DXMatrixPerspectiveFovLH(&matProj, 3.1415926f / 3.0f, 16.0f / 9.0f /*4.0f / 3.0f*/, 0.1f, 10000.0f);
	//if (cID != CAMERA_ID::PLAYER_CAMERA){
	//	Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 485), vector2(0.20f, 0.25f), 0.5f, "AngleH:" + std::to_string(Math::angle(mCameraParam.AngleH)) + "f");
	//	Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 500), vector2(0.20f, 0.25f), 0.5f, "AngleV:" + std::to_string(Math::angle(mCameraParam.AngleV)) + "f");
	//}
	if (!cameraMove)mCameraParam.InputAngle = vector3(0, 0, 0);
	//���͒l�����ƂɌv�Z
	mCameraParam.AngleH += mCameraParam.InputAngle.x * CAMERA_ANGLE_SPEED *  frameTime;
	mCameraParam.AngleV += mCameraParam.InputAngle.y * CAMERA_ANGLE_SPEED *  frameTime;
	//�N�����v
	if (mCameraParam.AngleH > Math::radian(180))mCameraParam.AngleH = Math::radian(-180);
	else if (mCameraParam.AngleH < Math::radian(-180))mCameraParam.AngleH = Math::radian(180);
	if (mCameraParam.AngleV > Math::radian(180))mCameraParam.AngleV = Math::radian(-180);
	else if (mCameraParam.AngleV < Math::radian(-180))mCameraParam.AngleV = Math::radian(180);

	//�e�X�g�t�H���g
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 485), vector2(0.20f, 0.25f), 0.5f, "AngleH:" + std::to_string(Math::angle(mCameraParam.AngleH)) + "f");
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 500), vector2(0.20f, 0.25f), 0.5f, "AngleV:" + std::to_string(Math::angle(mCameraParam.AngleV)) + "f");
	//�^�[�Q�b�g���v���C���[��菭����ɃZ�b�g
	//mCameraParam.Target = RCMatrix4::getPosition(mat) + RCVector3::normalize(RCMatrix4::getUp(mat)) * CAMERA_PLAYER_TARGET_HEIGHT;//* Time::DeltaTime;
	//�^�[�Q�b�g�̈ʒu�̃}�g���b�N�X�擾
	Matrix4 t = RCMatrix4::translate(mCameraParam.Target);
	//�v���C���[�̍����g��
	Vector3 left = RCVector3::normalize(RCMatrix4::getLeft(matrix));
	//�������ɏ㉺�̉�]�����߂�
	mCameraParam.AngleVSub = -Math::angle(mCameraParam.AngleV - angleC);
	Matrix4 pitchMat = RCQuaternion::rotate(left, 0);// mCameraParam.AngleVSub);
	//�����p(Player.h�Q��)
	angleC = mCameraParam.AngleV;
	//���̑O�ɂ������̉�]���|����
	Vector3 front = RCVector3::normalize(RCMatrix4::getFront(matrix) * pitchMat);
	//�O�ς�p���ď�����߂�
	Vector3 up = RCVector3::normalize(RCVector3::cross(front, left));
	//������ɍ��E�̉�]�����߂�
	mCameraParam.AngleHSub = Math::angle(mCameraParam.AngleH - angleF);
	Matrix4 yawMat = RCQuaternion::rotate(up, mCameraParam.AngleHSub);
	//�����p(Player.h�Q��)
	angleF = mCameraParam.AngleH;
	//�������O�ƍ������߂�
	front = RCVector3::normalize(RCVector3::cross(left, up)) * yawMat;// RCQuaternion::rotate(up, ;// *yawMat);
	left = RCVector3::normalize(RCVector3::cross(up, front));// RCQuaternion::rotate(up, ;// *yawMat);

	if (!Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_LSHIFT)){
		godShiftFront = front;
		godShiftLeft = left;
	}
	//left = left * yawMat;
	//��]�s����Z�b�g
	Matrix4 rotate = RCMatrix4::Identity();
	RCMatrix4::setLeft(rotate, left);
	RCMatrix4::setUp(rotate, up);
	RCMatrix4::setFront(rotate, front);
	matrix = rotate;
	godPos += godShiftFront * godFront * speed * frameTime + godShiftLeft *godLeft * speed * frameTime + vector3(0,1,0) * godUp * speed * frameTime;
	mCameraParam.Eye = godPos;
	mCameraParam.Target = mCameraParam.Eye + front * 3.0f;

	mCameraParam.Up = up;//* Time::DeltaTime;
	// �r���[�s��
	D3DXMatrixLookAtLH(&matView,
		&RConvert(&mCameraParam.Eye),
		&RConvert(&mCameraParam.Target),
		&RConvert(&mCameraParam.Up));
}


//�r���[�̃}�g���b�N�X���擾
D3DXMATRIX Camera::returnView(){
	return matView;
}
//�v���W�F�N�V�����̃}�g���b�N�X���擾
D3DXMATRIX Camera::returnProj(){
	return matProj;
}

void Camera::SetMatrix(Matrix4 mat_){
	mat = mat_;
}

CAMERA_PARAMETER* Camera::CameraParam(){
	return &mCameraParam;
}

Matrix4 Camera::ReturnCameraRotate(){
	return mat;
}

void Camera::SetPadNum(int padNum_){
	padNum = padNum_;
}

void Camera::SetCameraMove(bool cameraMove_){
	cameraMove = cameraMove_;
}

//InputAngle�̒l��ύX�iAI�p�j
void Camera::AddInputAngleHV(float h, float v)
{
	mCameraParam.InputAngle = vector3(h, v, 0);
}
//�J�����̉�]�s����擾
Matrix4 Camera::GetCameraRotMatrix()
{
	return matrix;
}