#pragma once
#include "../../Other/DX11User.h"
#include "../../Math/Vector3.h"
#include "../../Math/Matrix4.h"

enum CAMERA_ID;
class Stage;
#include <memory>

// �J�����̃p�����[�^�\����
struct CAMERA_PARAMETER
{
	float		AngleH;				// �����p�x
	float		AngleV;				// �����p�x
	float		AngleHSub;			//�����p�x�̍���
	float		AngleVSub;			//�����p�x�̍���
	Vector3		Eye;				// �J�������W
	Vector3		Target;				// �����_���W
	Vector3		Up;					// �����
	Vector3		InputAngle;			// ����
};

class Camera{
public:
	Camera(CAMERA_ID cID_,std::weak_ptr<Stage> stage_);
	~Camera();
	void Initialize();
	//�|�W�V�����ƃr���[���Z�b�g
	void SetCamera(Vector3 cameraPos, Vector3 cameraView, float frameTime);
	//�|�W�V�����ƃr���[���Z�b�g
	void GotCamera(float frameTime);
	//�r���[�̃}�g���b�N�X���擾
	D3DXMATRIX returnView();
	//�v���W�F�N�V�����̃}�g���b�N�X���擾
	D3DXMATRIX returnProj();

	void SetMatrix(Matrix4 pos);

	CAMERA_PARAMETER* CameraParam();

	Matrix4 ReturnCameraRotate();

	void SetPadNum(int padNum_);

	void SetCameraMove(bool cameraMove_);

	//InputAngle�̒l��ύX�iAI�p�j
	void AddInputAngleHV(float h, float v);
	//�J�����̉�]�s����擾
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