#pragma once
#include "../../Other/DX11User.h"
#include "../../Math/Vector3.h"
#include "../../Math/Matrix4.h"

enum CAMERA_ID;
class Stage;
#include <memory>

// カメラのパラメータ構造体
struct CAMERA_PARAMETER
{
	float		AngleH;				// 水平角度
	float		AngleV;				// 垂直角度
	float		AngleHSub;			//水平角度の差分
	float		AngleVSub;			//水平角度の差分
	Vector3		Eye;				// カメラ座標
	Vector3		Target;				// 注視点座標
	Vector3		Up;					// 上方向
	Vector3		InputAngle;			// 入力
};

class Camera{
public:
	Camera(CAMERA_ID cID_,std::weak_ptr<Stage> stage_);
	~Camera();
	void Initialize();
	//ポジションとビューをセット
	void SetCamera(Vector3 cameraPos, Vector3 cameraView, float frameTime);
	//ポジションとビューをセット
	void GotCamera(float frameTime);
	//ビューのマトリックスを取得
	D3DXMATRIX returnView();
	//プロジェクションのマトリックスを取得
	D3DXMATRIX returnProj();

	void SetMatrix(Matrix4 pos);

	CAMERA_PARAMETER* CameraParam();

	Matrix4 ReturnCameraRotate();

	void SetPadNum(int padNum_);

	void SetCameraMove(bool cameraMove_);

	//InputAngleの値を変更（AI用）
	void AddInputAngleHV(float h, float v);
	//カメラの回転行列を取得
	Matrix4 GetCameraRotMatrix();

	void SetDokusai(){ dokusai = true; }
	void SetPlayerAI(){ playerAI = true; }
	void SetIsRespawn(bool flag){ isRespawn = flag; }
private:
	bool isRespawn;
	std::weak_ptr<Stage> stage;
	D3DXMATRIX matView, matProj;
	CAMERA_PARAMETER mCameraParam;
	Matrix4 mat;
	float angleC,angleF;
	Matrix4 matrix;
	int padNum;
	bool cameraMove;
	CAMERA_ID cID;
	float siyaChange;
	bool dokusai,playerAI;

	Vector3 godPos;
	float godFront, godLeft,godUp;
	Vector3 godShiftFront, godShiftLeft;
	float speed = 3;
};