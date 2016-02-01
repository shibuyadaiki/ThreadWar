#include "Camera.h"
#include "../../Math/Converter.h"
#include "../../Other/Device.h"
#include ".././Collision.h"
#include "../../Graphic/Graphic.h"
#include "../Stage.h"
const float CAMERA_ANGLE_SPEED = 0.003f * 60.0f;		// 旋回速度
const float CAMERA_PLAYER_TARGET_HEIGHT = 0.75f;	// プレイヤー座標からどれだけ高い位置を注視点とするか
const float CAMERA_PLAYER_LENGTH = 2.5f;	// プレイヤーとの距離
const float CAMERA_COLLISION_SIZE = 0.2f;	// カメラの当たり判定サイズ

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
//ポジションとビューをセット
void Camera::SetCamera(Vector3 cameraPos, Vector3 cameraView, float frameTime){
	// 射影行列
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
	//入力値をもとに計算
	mCameraParam.AngleH += mCameraParam.InputAngle.x * sp * frameTime;
	mCameraParam.AngleV += mCameraParam.InputAngle.y * sp * frameTime;
	//クランプ
	if (mCameraParam.AngleH > Math::radian(180))mCameraParam.AngleH = Math::radian(-180);
	else if (mCameraParam.AngleH < Math::radian(-180))mCameraParam.AngleH = Math::radian(180);
	if (mCameraParam.AngleV > Math::radian(180))mCameraParam.AngleV = Math::radian(-180);
	else if (mCameraParam.AngleV < Math::radian(-180))mCameraParam.AngleV = Math::radian(180);

	//テストフォント
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 485), vector2(0.20f, 0.25f), 0.5f, "AngleH:" + std::to_string(Math::angle(mCameraParam.AngleH)) + "f");
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 500), vector2(0.20f, 0.25f), 0.5f, "AngleV:" + std::to_string(Math::angle(mCameraParam.AngleV)) + "f");
	//ターゲットをプレイヤーより少し上にセット
	mCameraParam.Target = RCMatrix4::getPosition(mat) + cameraPos + RCVector3::normalize(RCMatrix4::getUp(mat)) * CAMERA_PLAYER_TARGET_HEIGHT;//* Time::DeltaTime;

	//ターゲットの位置のマトリックス取得
	Matrix4 t = RCMatrix4::translate(mCameraParam.Target);
	//プレイヤーの左を使う
	Vector3 left = RCVector3::normalize(RCMatrix4::getLeft(mat));
	//左を軸に上下の回転を求める
	mCameraParam.AngleVSub = Math::angle(mCameraParam.AngleV - angleC);
	Matrix4 pitchMat = RCQuaternion::rotate(left, mCameraParam.AngleVSub);
	//差分用(Player.h参照)
	angleC = mCameraParam.AngleV;
	//今の前にさっきの回転を掛ける
	Vector3 front = RCVector3::normalize(RCMatrix4::getFront(matrix) * pitchMat);
	//外積を用いて上を求める
	Vector3 up = RCVector3::normalize(RCVector3::cross(front, left));
	//上を軸に左右の回転を求める
	mCameraParam.AngleHSub = Math::angle(mCameraParam.AngleH - angleF);
	Matrix4 yawMat = RCQuaternion::rotate(up, mCameraParam.AngleHSub);
	//差分用(Player.h参照)
	angleF = mCameraParam.AngleH;
	//正しい前と左を求める
	front = RCVector3::normalize(RCVector3::cross(left, up));// *yawMat);
	//left = left * yawMat;

	//後ろに下げる
	Matrix4 trans = RCMatrix4::translate(front * CAMERA_PLAYER_LENGTH);
	//Targetの位置から後ろに下げる
	Matrix4 x = t* trans;
	//プレイヤーのけつ方向とカメラ方向のなす角
	float angle = Math::acos(RCVector3::dot(RCVector3::normalize(RCMatrix4::getFront(mat)), front));
	//回転行列をセット
	Matrix4 rotate = RCMatrix4::Identity();
	RCMatrix4::setLeft(rotate, left);
	RCMatrix4::setUp(rotate, up);
	RCMatrix4::setFront(rotate, front);
	matrix = rotate;

	//プレイヤーのけつ方向とカメラ方向のなす角が70度以上なら
	if (angle > 70.0f){
		//１度ずつ引いて求める
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

	//カメラとのあたり判定(２分探索法を用いて距離を短くしていく)
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
	// ビュー行列
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
	//入力値をもとに計算
	mCameraParam.AngleH += mCameraParam.InputAngle.x * CAMERA_ANGLE_SPEED *  frameTime;
	mCameraParam.AngleV += mCameraParam.InputAngle.y * CAMERA_ANGLE_SPEED *  frameTime;
	//クランプ
	if (mCameraParam.AngleH > Math::radian(180))mCameraParam.AngleH = Math::radian(-180);
	else if (mCameraParam.AngleH < Math::radian(-180))mCameraParam.AngleH = Math::radian(180);
	if (mCameraParam.AngleV > Math::radian(180))mCameraParam.AngleV = Math::radian(-180);
	else if (mCameraParam.AngleV < Math::radian(-180))mCameraParam.AngleV = Math::radian(180);

	//テストフォント
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 485), vector2(0.20f, 0.25f), 0.5f, "AngleH:" + std::to_string(Math::angle(mCameraParam.AngleH)) + "f");
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 500), vector2(0.20f, 0.25f), 0.5f, "AngleV:" + std::to_string(Math::angle(mCameraParam.AngleV)) + "f");
	//ターゲットをプレイヤーより少し上にセット
	//mCameraParam.Target = RCMatrix4::getPosition(mat) + RCVector3::normalize(RCMatrix4::getUp(mat)) * CAMERA_PLAYER_TARGET_HEIGHT;//* Time::DeltaTime;
	//ターゲットの位置のマトリックス取得
	Matrix4 t = RCMatrix4::translate(mCameraParam.Target);
	//プレイヤーの左を使う
	Vector3 left = RCVector3::normalize(RCMatrix4::getLeft(matrix));
	//左を軸に上下の回転を求める
	mCameraParam.AngleVSub = -Math::angle(mCameraParam.AngleV - angleC);
	Matrix4 pitchMat = RCQuaternion::rotate(left, 0);// mCameraParam.AngleVSub);
	//差分用(Player.h参照)
	angleC = mCameraParam.AngleV;
	//今の前にさっきの回転を掛ける
	Vector3 front = RCVector3::normalize(RCMatrix4::getFront(matrix) * pitchMat);
	//外積を用いて上を求める
	Vector3 up = RCVector3::normalize(RCVector3::cross(front, left));
	//上を軸に左右の回転を求める
	mCameraParam.AngleHSub = Math::angle(mCameraParam.AngleH - angleF);
	Matrix4 yawMat = RCQuaternion::rotate(up, mCameraParam.AngleHSub);
	//差分用(Player.h参照)
	angleF = mCameraParam.AngleH;
	//正しい前と左を求める
	front = RCVector3::normalize(RCVector3::cross(left, up)) * yawMat;// RCQuaternion::rotate(up, ;// *yawMat);
	left = RCVector3::normalize(RCVector3::cross(up, front));// RCQuaternion::rotate(up, ;// *yawMat);

	if (!Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_LSHIFT)){
		godShiftFront = front;
		godShiftLeft = left;
	}
	//left = left * yawMat;
	//回転行列をセット
	Matrix4 rotate = RCMatrix4::Identity();
	RCMatrix4::setLeft(rotate, left);
	RCMatrix4::setUp(rotate, up);
	RCMatrix4::setFront(rotate, front);
	matrix = rotate;
	godPos += godShiftFront * godFront * speed * frameTime + godShiftLeft *godLeft * speed * frameTime + vector3(0,1,0) * godUp * speed * frameTime;
	mCameraParam.Eye = godPos;
	mCameraParam.Target = mCameraParam.Eye + front * 3.0f;

	mCameraParam.Up = up;//* Time::DeltaTime;
	// ビュー行列
	D3DXMatrixLookAtLH(&matView,
		&RConvert(&mCameraParam.Eye),
		&RConvert(&mCameraParam.Target),
		&RConvert(&mCameraParam.Up));
}


//ビューのマトリックスを取得
D3DXMATRIX Camera::returnView(){
	return matView;
}
//プロジェクションのマトリックスを取得
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

//InputAngleの値を変更（AI用）
void Camera::AddInputAngleHV(float h, float v)
{
	mCameraParam.InputAngle = vector3(h, v, 0);
}
//カメラの回転行列を取得
Matrix4 Camera::GetCameraRotMatrix()
{
	return matrix;
}