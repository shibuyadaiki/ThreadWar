#include "FirstAI.h"
#include "../../Audio/Audio.h"
#include "../Thread.h"
#include "../CrystalCenter.h"
#include "../Collision.h"

//����p(���g��flont����̊p�x)
static const float viewAngle = 70.0f;
//���싗��
static const float viewLength = 50.0f;
//�U���͈͊p(�J������flont����̊p�x)
static const float attackAngle = 10.0f;
//�U���͈͋���
static const float attackLength = 40.0f;
//��������Đڋ߂��鎞�̍ŒZ����(�����艓���ꍇ�͊�������Đڋ�)
static const float rewindLength = 30.0f;
//���z����
static const float bestLength = 15.0f;
//�����I�ɐ퓬��ԂɈڍs���鋗��
static const float dangerLength = 10.0f;

FirstAI::FirstAI(IWorld& wo, PlayerActionManager& action, std::weak_ptr<Player> player, CAMERA_ID& cID_, ActorParameter& parameter_, std::weak_ptr<Stage> stage_) :
EnemyAI(wo, action, player, cID_, parameter_),
stage(stage_){
	Initialize();
}

void FirstAI::Initialize(){
	nextAI = AI_STATE::FIRST_AI;
	teamID = player._Get()->GetTeam();

	aiTimer = Math::random(1.0f, 5.0f);
	lostTimer = 0.0f;

	for (int i = 0; i < 4; i++){
		targetMats[i] = RCMatrix4::Identity();
	}
	target.mat = RCMatrix4::Identity();
	target.pos = vector3(0, 0, 0);
	target.vec = vector3(0, 0, 0);
	target.isLooking = false;
	target.lastLookingPos = vector3(0, 0, 0);
	target.lastLookingVec = vector3(0, 0, 0);
	target.distance = 0.0f;
	target.isBattle = false;

	flag = false;
	timer = 0.0f;
	counter = 0;

	moveTime = stepTime = threadTime = 0;
	moveTimeMax = Math::random(1.0f, 5.0f);
	stepTimeMax = Math::random(1.0f, 3.0f);
	threadTimeMax = Math::random(1.0f, 2.0f);
	inputVecX = 1.0f;
	inputCameraX = 1.0f;

	myMat = player._Get()->GetParameter().matrix;
	myPos = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);
	myPos2D = vector2(myPos.x, myPos.z);
	myParam = player._Get()->ReturnPlayerParameter();

	camera = Device::GetInstance().GetCamera(cID);
	cameraRotMat = RCMatrix4::Identity();
	cameraFront = vector3(0, 0, 0);
	cameraLeft = vector3(0, 0, 0);
	cameraUp = vector3(0, 0, 0);
	inputAngle = vector2(0, 0);
	inputVec = vector3(0, 0, 0);

	centerCrystalDist = iceCrystalDist = caveCrystalDist = nearestCrystalDist = 0.0f;
	nearestCrystalPos = vector3(0, 0, 0);
	targetCrystal = false;

	//�T�[�`�Ώ�
	if (parameter.id == ACTOR_ID::ENEMY_ACTOR) searchActor = ACTOR_ID::PLAYER_ACTOR;
	else searchActor = ACTOR_ID::ENEMY_ACTOR;

	actionFunc.clear();
	actionFunc[AI_ACTION::Curl] = std::bind(&FirstAI::Curl, this, std::placeholders::_1);
	actionFunc[AI_ACTION::RandomMove] = std::bind(&FirstAI::RandomMove, this, std::placeholders::_1);
	actionFunc[AI_ACTION::RandomLook] = std::bind(&FirstAI::RandomLook, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TurnLook] = std::bind(&FirstAI::TurnLook, this, std::placeholders::_1);
	actionFunc[AI_ACTION::JumpShotRewind] = std::bind(&FirstAI::JumpShotRewind, this, std::placeholders::_1);
	actionFunc[AI_ACTION::FrontLook] = std::bind(&FirstAI::FrontLook, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointLookH] = std::bind(&FirstAI::TargetPointLookH, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointLookV] = std::bind(&FirstAI::TargetPointLookV, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointLook] = std::bind(&FirstAI::TargetPointLook, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointLookAttack] = std::bind(&FirstAI::TargetPointLookAttack, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointMove] = std::bind(&FirstAI::TargetPointMove, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointMoveAttack] = std::bind(&FirstAI::TargetPointMoveAttack, this, std::placeholders::_1);
	actionFunc[AI_ACTION::CrystalPointMoveAttack] = std::bind(&FirstAI::CrystalPointMoveAttack, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetJumpShotRewind] = std::bind(&FirstAI::TargetJumpShotRewind, this, std::placeholders::_1);
	actionFunc[AI_ACTION::GettingThread] = std::bind(&FirstAI::GettingThread, this, std::placeholders::_1);
	actionFunc[AI_ACTION::CreateThreadWeb] = std::bind(&FirstAI::CreateThreadWeb, this, std::placeholders::_1);

	for (int i = 0; i < 3; i++)
	{
		level[i].viewAngle = 35.0f * (i + 1);
		level[i].viewLength = 25.0f * (i + 1);
		level[i].attackAngle = level[i].dangerLength = 5.0f * (i + 1);
		level[i].attackLength = 20.0f * (i + 1);
		level[i].rewindLength = 30.0f;
		level[i].bestLength = 15.0f;
		level[i].penetrationTime = 1.0f * (i + 1);
		level[i].inputXChangeTimeMax = 10.0f / (i + 1);
		level[i].inputXChangeTimeMin = 2.0f / (i + 1);
		level[i].stepTimeMax = 6.0f / (i + 1);
		level[i].stepTimeMin = 2.0f / (i + 1);
		level[i].battleStepTimeMax = 4.0f / (i + 1);
		level[i].battleStepTimeMin = 2.0f / (i + 1);
	}
	levelNum = 1;
}

Vector3 FirstAI::UpdateAI(float frameTime){
	//���͏�ԃ��Z�b�g
	InputReset();

	//�p�����[�^���AAI���s�O�ɕK�v�Ȍv�Z
	ParamCalc();

	//AI�����s
	AIRun(frameTime);
	//stateFunc[currentAction](frameTime);
	//GettingThread(frameTime);

	//�ŏI�I�ȓ���
	FinalInput();

	return inputVec;
}

void FirstAI::ParamCalc()
{
	//���g�̃p�����[�^���擾
	myMat = player._Get()->GetParameter().matrix;
	myPos = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);
	myPos2D = vector2(myPos.x, myPos.z);
	myParam = player._Get()->ReturnPlayerParameter();

	//�J�����̎擾
	//camera = Device::GetInstance().GetCamera(cID);
	cameraRotMat = camera->GetCameraRotMatrix();
	cameraFront = -RCVector3::normalize(RCMatrix4::getFront(cameraRotMat));
	cameraLeft = RCVector3::normalize(RCMatrix4::getLeft(cameraRotMat));
	cameraUp = RCVector3::normalize(RCMatrix4::getUp(cameraRotMat));

	//�e�N���X�^���܂ł̋���
	centerCrystalDist = RCVector3::distance_(myPos, centerCrystalPos);
	iceCrystalDist = RCVector3::distance_(myPos, iceCrystalPos);
	caveCrystalDist = RCVector3::distance_(myPos, caveCrystalPos);
	//��ԋ߂��U���ΏۃN���X�^�������
	targetCrystal = true;
	if (parameter.id == ACTOR_ID::PLAYER_ACTOR){
		if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR)._Get()->IsPlayer() &&
			caveCrystalDist < centerCrystalDist && caveCrystalDist < iceCrystalDist){
			nearestCrystalPos = caveCrystalPos;
			nearestCrystalDist = caveCrystalDist;
		}
		else if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_CENTER_ACTOR)._Get()->IsPlayer() &&
			centerCrystalDist < iceCrystalDist){
			nearestCrystalPos = centerCrystalPos;
			nearestCrystalDist = centerCrystalDist;
		}
		else if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR)._Get()->IsPlayer()){
			nearestCrystalPos = iceCrystalPos;
			nearestCrystalDist = iceCrystalDist;
		}
		else
			targetCrystal = false;
	}
	else{
		if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR)._Get()->IsEnemy() &&
			iceCrystalDist < centerCrystalDist && iceCrystalDist < caveCrystalDist){
			nearestCrystalPos = iceCrystalPos;
			nearestCrystalDist = iceCrystalDist;
		}
		else if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_CENTER_ACTOR)._Get()->IsEnemy() &&
			centerCrystalDist < caveCrystalDist){
			nearestCrystalPos = centerCrystalPos;
			nearestCrystalDist = centerCrystalDist;
		}
		else if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR)._Get()->IsEnemy()){
			nearestCrystalPos = caveCrystalPos;
			nearestCrystalDist = caveCrystalDist;
		}
		else
			targetCrystal = false;
	}

	//���͏�����
	inputAngle = vector2(0, 0);
	inputVec = vector3(0, 0, 0);
}

void FirstAI::AIRun(float frameTime)
{
	//�G�̈ʒu������
	int num = 0;
	world.EachActor(searchActor, [&](const Actor& other){
		targetMats[num] = static_cast<Player*>(const_cast<Actor*>(&other))->GetParameter().matrix;
		targetDists[num] = RCVector3::distance_(myPos, RCMatrix4::getPosition(targetMats[num]));
		num++;
	});

	//���E���ɖڕW�����邩���ׂ�
	target.isLooking = Search();

	//�ڕW�𔭌�����܂ł̓N���X�^�����U��or���G
	if (!target.isBattle)
	{
		if (targetCrystal)
		{
			//�^�[�Q�b�g���N���X�^����
			target.lastLookingPos = nearestCrystalPos;
			target.lastLookingVec = RCVector3::normalize(nearestCrystalPos - myPos);
			target.distance = nearestCrystalDist;
			//CrystalPointMoveAttack(frameTime);
			ChangeAction(AI_ACTION::CrystalPointMoveAttack);
		}
		else
		{
			aiTimer += frameTime;
			if (aiTimer < 8.0f)
				ChangeAction(AI_ACTION::RandomMove);
			//RandomMove(frameTime);
			else if (aiTimer < 11.0f)
				ChangeAction(AI_ACTION::CreateThreadWeb);
			//CreateThreadWeb(frameTime);
			else if (aiTimer < 14.0f)
				ChangeAction(AI_ACTION::JumpShotRewind);
			//JumpShotRewind(frameTime);
			else
			{
				ChangeAction(AI_ACTION::RandomMove);
				aiTimer = 0.0f;
				//flag = false;
				//timer = 0.0f;
			}
		}

		//�G�𔭌������ꍇ
		if (target.isLooking)
		{
			//�������N���X�^�����߂��ꍇ
			if (targetDists[targetNum] < nearestCrystalDist)
			{
				//�퓬��Ԃֈڍs
				target.isBattle = true;
				aiTimer = 0.0f;
			}
		}
	}

	//�퓬��Ԃ̏ꍇ
	if (target.isBattle)
	{
		//���E���ɑ����Ă���Ȃ�
		if (target.isLooking)
		{
			//�Ō�ɑ������n�_���X�V
			target.lastLookingPos = target.pos;
			//�������擾
			target.lastLookingVec = RCVector3::normalize(target.lastLookingPos - myPos);
			lostTimer = 0;
		}
		//�����Ă��Ȃ��Ȃ�
		else
		{
			//�����������Ԃɉ��Z
			lostTimer += frameTime;
			//2�b�ȓ��͊ђʂ��Č�����
			if (lostTimer <= 2.0f){
				target.lastLookingPos = target.pos;
				target.lastLookingVec = RCVector3::normalize(target.lastLookingPos - myPos);
			}
		}

		if (lostTimer <= 4.0f)
		{
			//�������߂��A�ڒn���Ă���Ȃ�
			if (myParam->onGroundFlag && target.distance <= bestLength)
				//�����˂��Ȃ���ړ�
				ChangeAction(AI_ACTION::TargetPointMoveAttack);
			//TargetPointMoveAttack(frameTime);
			//�����������Ȃ�
			else
				//��������Đڋ�
				ChangeAction(AI_ACTION::TargetJumpShotRewind);
			//TargetJumpShotRewind(frameTime);
		}
	}

	//�S�b�ȏ㌩��������
	if (lostTimer > 4.0f)
	{
		//�Ō�ɑ������n�_�܂ňړ�
		float dist = RCVector2::distance_(myPos2D, vector2(target.lastLookingPos.x, target.lastLookingPos.z));
		//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 180), vector2(0.20f, 0.25f), 0.5f, "lost:" + std::to_string(lostTimer));
		if (lostTimer  < 12.0f)
			//8�b�Ԉړ�
			ChangeAction(AI_ACTION::TargetPointMove);
		//TargetPointMove(frameTime);
		else
		{
			//�ړ����I��������񂵂č��G
			aiTimer += frameTime;
			if (aiTimer < 6.0f)
				//���G�̂��߂ɉ�]
				ChangeAction(AI_ACTION::TurnLook);
			//TurnLook(frameTime);
			else if (aiTimer >= 6.0f)
			{
				//��]���Ă�������Ȃ������ꍇ�͐퓬��ԉ���
				target.isBattle = false;
				lostTimer = 0;
				aiTimer = 0;
			}
		}
	}

	actionFunc[currentAction](frameTime);

	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 400), vector2(0.20f, 0.25f), 0.5f, "looking:" + std::to_string(target.isLooking));
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 380), vector2(0.20f, 0.25f), 0.5f, "attack:" + std::to_string(AttackArea()));
}

void FirstAI::InputReset()
{
	player._Get()->SetInputCurlOn(false);
	player._Get()->SetInputCurlOff(false);
	player._Get()->SetInputJump(false);
	player._Get()->SetInputStep(false);
	player._Get()->SetInputShot(false);
}

void FirstAI::ChangeAction(AI_ACTION s)
{
	if (currentAction != s)
	{
		currentAction = s;
		flag = false;
		timer = 0.0f;
		counter = 0;
	}
}
bool FirstAI::IsCeiling()
{
	return RCV3Calc::InnerAngle(RCVector3::normalize(RCMatrix4::getUp(cameraRotMat)), vector3(0, 1, 0)) > 90;
}
void FirstAI::Curl(float frameTime)
{
	if (!flag)
	{
		InputStep();
		InputCurlOn();
		flag = true;
	}

	timer += frameTime;
	if (timer >= 3.0f)
	{
		InputCurlOff();
	}
}
void FirstAI::RandomMove(float frameTime)
{
	//�^�C�}����
	moveTime += frameTime;
	stepTime += frameTime;

	//�O�i����
	inputVec.z = -1;
	//���E����
	inputVec.x = inputVecX;
	inputVec = RCVector3::normalize(inputVec);
	//���E���͐ؑ�
	if (moveTime > moveTimeMax){
		inputVecX = -inputVecX;
		moveTime = 0;
		moveTimeMax = Math::random(1.0f, 5.0f);
	}

	//�X�e�b�v����
	if (stepTime > stepTimeMax){
		InputStep();
		stepTime = 0;
		stepTimeMax = Math::random(1.0f, 3.0f);
	}
}
void FirstAI::RandomLook(float frameTime)
{
	if (!flag)
	{
		moveTimeMax = Math::random(1.0f, 3.0f);	//���͂̒���
		inputCameraX = Math::random(3.0f, 9.0f);	//���͂̋���
		if (Math::random(-1.0f, 1.0f) < 0) inputVecX *= -1;	//1/2�̊m���œ��͕������]
		flag = true;
	}

	//�^�C�}����
	timer += frameTime;
	//�J��������
	inputAngle.x = inputCameraX;

	if (timer > moveTimeMax)
	{
		//�������
		flag = false;
		timer = 0.0f;
	}
}
void FirstAI::TurnLook(float frameTime)
{
	if (!flag)
	{
		inputCameraX = 3.0f;	//���͂̋���
		flag = true;
	}

	//�J��������
	inputAngle.x = inputCameraX;
}
void FirstAI::JumpShotRewind(float frameTime)
{
	timer += frameTime;
	inputVec.z = -1.0f;
	FrontLook(frameTime);
	if (timer < 1.0f) return;

	//��b��������W�����v�����ˁ������
	switch (counter)
	{
	case 0:
		InputJump();
		counter++;
		break;

	case 1:
		if (timer > 1.3f)
		{
			InputShotThread();
			counter++;
		}
		break;

	case 2:
		if (timer > 1.4f)
		{
			InputRewind();
			counter++;
		}
		break;
	case 3:
		//�������̐���/���s�ɂ�����炸�A�ڒn���Ă�����
		if (myParam->onGroundFlag)
		{
			//������
			counter = 0;
			timer = 0;
		}
	default:
		break;
	}

}
void FirstAI::SlantingShotRewind(float frameTime)
{
	if (counter >= 1)
		timer += frameTime;

	float angle = Math::acos(RCVector3::dot(cameraFront, RCVector3::normalize(RCMatrix4::getUp(myMat))));
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 120), vector2(0.20f, 0.25f), 0.5f, "angle:" + std::to_string(angle));

	//�΂ߏ���������犪������Ĉړ�
	switch (counter)
	{
	case 0:
		//���g�̏�����ɃJ������������
		if (angle > 30)
		{
			inputAngle.y += 6.0f;
			break;
		}
		counter++;
		break;

	case 1:
		//�ŏ��ɃW�����v
		InputJump();
		counter++;
		break;

	case 2:
		if (timer > 0.3f)
		{
			//����
			InputShotThread();
			counter++;
		}
		break;

	case 3:
		if (timer > 0.5f)
		{
			//�������
			InputRewind();
			counter++;
		}
		break;

	default:
		break;
	}
}
void FirstAI::LookV(float frameTime, Vector3 v)
{
	//������������
	Vector3 vec = v;

	if (vec.z > 0)
		vec = -vec;//+�̒l�ɐ������Čv�Z

	//�㉺����
	float asin = Math::asin(RCVector2::cross(
		RCVector2::normalize(vector2(cameraFront.y, cameraFront.z)),
		RCVector2::normalize(vector2(vec.y, vec.z))));

	//�Ȃ��p
	float acos = Math::acos(RCVector3::dot(
		RCVector3::normalize(vector3(0, cameraFront.y, cameraFront.z)),
		RCVector3::normalize(vector3(0, vec.y, target.vec.z))));

	//�Ȃ��p����ɓ��͗ʂ��v�Z
	float add = fabsf(acos) / 2;
	add = Math::clamp(add, 1.0f, 10.0f);

	//���͂ɂ�������ς���
	if (asin < 0) inputAngle.y = -add;
	else inputAngle.y = add;

	//�V��ɂ���ꍇ�͓��͔��]
	if (IsCeiling()) inputAngle.y *= -1;
}
void FirstAI::FrontLook(float frameTime)
{
	LookV(frameTime, RCMatrix4::getFront(myMat));
}
void FirstAI::TargetPointLookH(float frameTime)
{
	//���E����
	float asin = Math::asin(RCVector2::cross(
		RCVector2::normalize(vector2(cameraFront.x, cameraFront.z)),
		RCVector2::normalize(vector2(target.lastLookingVec.x, target.lastLookingVec.z))));

	//�Ȃ��p
	float acos = RCV3Calc::InnerAngle(
		vector3(cameraFront.x, 0, cameraFront.z),
		vector3(target.lastLookingVec.x, 0, target.lastLookingVec.z));

	//�Ȃ��p����ɓ��͗ʂ��v�Z
	float add = fabsf(acos) / 2;
	add = Math::clamp(add, 1.0f, 10.0f);

	//���͂ɂ�������ς���
	if (asin > 0) inputAngle.x = -add;
	else inputAngle.x = add;

	//�V��ɂ���ꍇ�͓��͔��]
	if (IsCeiling()) inputAngle.x *= -1;
}
void FirstAI::TargetPointLookV(float frameTime)
{
	//������������
	Vector3 vec = target.lastLookingVec;

	if (vec.z > 0)
		vec = -vec;//+�̒l�ɐ������Čv�Z

	//�㉺����
	float asin = Math::asin(RCVector2::cross(
		RCVector2::normalize(vector2(cameraFront.y, cameraFront.z)),
		RCVector2::normalize(vector2(vec.y, vec.z))));

	//�Ȃ��p
	float acos = RCV3Calc::InnerAngle(
		vector3(0, cameraFront.y, cameraFront.z),
		vector3(0, vec.y, target.vec.z));

	//�Ȃ��p����ɓ��͗ʂ��v�Z
	float add = fabsf(acos) / 2;
	add = Math::clamp(add, 1.0f, 10.0f);

	//���͂ɂ�������ς���
	if (asin < 0) inputAngle.y = -add;
	else inputAngle.y = add;

	//�V��ɂ���ꍇ�͓��͔��]
	if (IsCeiling()) inputAngle.y *= -1;
}
void FirstAI::TargetPointLook(float frameTime)
{
	TargetPointLookH(frameTime);
	TargetPointLookV(frameTime);
}
void FirstAI::TargetPointLookAttack(float frameTime)
{
	TargetPointLookH(frameTime);
	TargetPointLookV(frameTime);

	timer += frameTime;
	if (timer >= 0.5f)
	{
		InputShotThread();
		timer = 0;
	}
}
void FirstAI::TargetPointMove(float frameTime)
{
	TargetPointLookH(frameTime);
	TargetPointLookV(frameTime);
	//�O�i
	inputVec.z = -1;

	//�^�C�}����
	stepTime += frameTime;
	//�X�e�b�v����
	if (stepTime > stepTimeMax){
		InputStep();
		stepTime = 0;
		stepTimeMax = Math::random(1.0f, 3.0f);
	}
}
void FirstAI::TargetPointMoveAttack(float frameTime)
{
	TargetPointLookH(frameTime);
	TargetPointLookV(frameTime);
	//�^�C�}����
	moveTime += frameTime;
	stepTime += frameTime;

	//�����ɂ���đO��̈ړ�������ς���
	if (target.distance > bestLength - 5)
		//�O�i
		inputVec.z = -1;
	else
		//���
		inputVec.z = 1;

	//���E���͐ؑ�
	if (moveTime > moveTimeMax){
		inputVecX = -inputVecX;
		moveTime = 0;
		moveTimeMax = Math::random(1.0f, 5.0f);
	}
	//���E����
	inputVec.x = inputVecX;
	//���K��
	inputVec = RCVector3::normalize(inputVec);


	//�X�e�b�v����
	if (stepTime > stepTimeMax){
		InputStep();
		stepTime = 0;
		stepTimeMax = Math::random(1.0f, 3.0f);
	}

	//���̔���
	timer += frameTime;

	if (timer > 0.7f)
	{
		InputShotThread();
		timer = 0;
	}
}
void FirstAI::CrystalPointMoveAttack(float frameTime)
{
	TargetPointLookH(frameTime);
	TargetPointLookV(frameTime);
	//�^�C�}����
	moveTime += frameTime;
	stepTime += frameTime;


	//�����ɂ���đO��̈ړ�������ς���
	if (target.distance > bestLength)
		//�O�i
		inputVec.z = -1;
	else
		//���
		inputVec.z = 1;

	//���E���͐ؑ�
	if (moveTime > moveTimeMax){
		inputVecX = -inputVecX;
		moveTime = 0;
		moveTimeMax = Math::random(1.0f, 5.0f);
	}
	//���E����
	inputVec.x = inputVecX;
	//���K��
	inputVec = RCVector3::normalize(inputVec);


	//�X�e�b�v����
	if (stepTime > stepTimeMax){
		InputStep();
		stepTime = 0;
		stepTimeMax = Math::random(1.0f, 3.0f);
	}

	//���̔���
	timer += frameTime;

	if (timer > 0.2f)
	{
		Vector3 start = myPos + RCMatrix4::getUp(myMat) * 3.0f;
		Vector3 end = nearestCrystalPos;
		end = end - RCVector3::normalize(end - start) * 2.0f;
		//Graphic::GetInstance().DrawLine(start, end, CAMERA_ID::PLAYER_CAMERA_1P);
		CollisionParameter stageCol = ModelRay(*stage._Get()->ReturnMat(), OCT_ID::STAGE_OCT, start, end);
		if (!stageCol.colFlag)
			InputShotThread();
		timer = 0;
	}
}
void FirstAI::TargetJumpShotRewind(float frameTime)
{
	timer += frameTime;
	//�ڕW�������Ȃ���O�i
	TargetPointLook(frameTime);
	inputVec.z = -1.0f;

	if (timer < 1.0f) return;

	//��b��������W�����v�����ˁ������
	switch (counter)
	{
	case 0:
		InputJump();
		counter++;
		break;

	case 1:
		if (timer > 1.3f)
		{
			InputShotThread();
			counter++;
		}
		break;

	case 2:
		if (timer > 1.4f)
		{
			InputRewind();
			counter++;
		}
		break;
	case 3:
		//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 150), vector2(0.20f, 0.25f), 0.5f, "jump:" + std::to_string(target.distance < bestLength));
		if (target.distance < bestLength)
		{
			InputJump();
			counter++;
		}
		if (timer > 5.0f)
		{
			timer = 0;
			counter = 0;
		}
		break;
	default:
		counter = 0;
		timer = 0;
		break;
	}
}
void FirstAI::GettingThread(float frameTime)
{
	if (counter >= 1)
		timer += frameTime;

	float angle = RCV3Calc::InnerAngle(cameraFront, RCMatrix4::getUp(myMat));

	//�΂ߏ���������玅���ˁ����ɏ��
	switch (counter)
	{
	case 0:
		//���g�̏�����ɃJ������������
		if (angle > 30)
		{
			inputAngle.y += 6.0f;
			break;
		}
		counter++;
		break;

	case 1:
		if (timer > 0.6f)
		{
			//����
			InputShotThread();
			counter++;
		}
		break;

	case 2:
		if (timer > 1.2f)
		{
			////�������
			//InputRewind();
			counter++;
		}
		break;

	default:
		break;
	}
}
void FirstAI::CreateThreadWeb(float frameTime)
{
	//�J�������͂ɂ���]
	inputAngle.x = 10.0f;
	//�Ə��𐳖ʂ�菭����Ɍ�����
	FrontLook(frameTime);

	//������
	threadTime += frameTime;
	if (threadTime >= 0.2f)
	{
		InputShotThread();
		threadTime = 0;
	}
}

bool FirstAI::Search()
{
	for (int i = 0; i < teamMemberCount; i++)
	{
		//�ڕW�̃}�g���b�N�X����v�Z
		Matrix4 mat = targetMats[i];
		Vector3 pos = RCMatrix4::getPosition(mat);
		Vector3 vec = RCVector3::normalize(pos - myPos);
		float dist = targetDists[i];
		//�����I�ɐ퓬��Ԃɓ��鋗���ɓG�����邩
		if (dist < dangerLength)
		{
			//target���m��
			target.mat = mat;
			target.pos = pos;
			target.vec = vec;
			target.distance = dist;
			targetNum = i;
			return true;
		}

		//���싗���ȓ��ɂ��邩
		//����p���ɂ��邩
		Vector3 front = RCVector3::normalize(-RCMatrix4::getFront(myMat));
		float acos = Math::acos(RCVector3::dot(front, vec));
		//�G���玩�g�̊Ԃ̐����ɃX�e�[�W�̃|���S�������邩
		Vector3 start = myPos + RCMatrix4::getUp(myMat) * 3.0f;
		Vector3 end = pos + RCMatrix4::getUp(mat) * 3.0f;
		CollisionParameter stageCol = ModelRay(*stage._Get()->ReturnMat(), OCT_ID::STAGE_OCT, start, end);
		if (dist < level[levelNum].viewLength && acos < level[levelNum].viewAngle && !stageCol.colFlag)
		{
			//target���m��
			target.mat = mat;
			target.pos = pos;
			target.vec = vec;
			target.distance = dist;
			targetNum = i;
			return true;
		}
	}
	return false;
}

void FirstAI::SearchLineDraw()
{
	//Vector3 playerFront = -RCVector3::normalize(RCMatrix4::getFront(myMat));
	//Vector3 playerUp = RCVector3::normalize(RCMatrix4::getUp(myMat));
	//Vector3 playerLeft = RCVector3::normalize(RCMatrix4::getLeft(myMat));
	////�㉺(x����])
	//Matrix4 rollmat = RCQuaternion::rotate(playerLeft, viewAngle);
	//Vector3 frontRise = RCVector3::normalize(playerFront * rollmat);
	//rollmat = RCQuaternion::rotate(playerLeft, -viewAngle);
	//Vector3 frontFall = RCVector3::normalize(playerFront * rollmat);
	////���E(y����])
	//Matrix4 pitchmat = RCQuaternion::rotate(playerUp, viewAngle);
	//Vector3 frontTurnRight = RCVector3::normalize(playerFront * pitchmat);
	//pitchmat = RCQuaternion::rotate(playerUp, -viewAngle);
	//Vector3 frontTurnLeft = RCVector3::normalize(playerFront * pitchmat);

	//Graphic::GetInstance().DrawLine(myPos, myPos + frontRise * viewLength, CAMERA_ID::ENEMY_CAMERA, vector3(0, 1, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontFall * viewLength, CAMERA_ID::ENEMY_CAMERA, vector3(0, 1, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontTurnRight * viewLength, CAMERA_ID::ENEMY_CAMERA, vector3(0, 1, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontTurnLeft * viewLength, CAMERA_ID::ENEMY_CAMERA, vector3(0, 1, 0));
}

bool FirstAI::AttackArea()
{
	//�@�U�������ȓ��ɂ��邩
	if (target.distance > attackLength) return false;

	//�A����p���ɂ��邩
	//AttackAreaLineDraw();
	float acos = Math::acos(RCVector3::dot(cameraFront, target.vec));
	if (acos > attackAngle) return false;

	return true;
}

void FirstAI::AttackAreaLineDraw()
{
	//Vector3 cameraPos = camera->CameraParam()->Eye;
	////�㉺(x����])
	//Matrix4 rollmat = RCQuaternion::rotate(cameraLeft, attackAngle);
	//Vector3 frontRise = RCVector3::normalize(cameraFront * rollmat);
	//rollmat = RCQuaternion::rotate(cameraLeft, -attackAngle);
	//Vector3 frontFall = RCVector3::normalize(cameraFront * rollmat);
	////���E(y����])
	//Matrix4 pitchmat = RCQuaternion::rotate(cameraUp, attackAngle);
	//Vector3 frontTurnRight = RCVector3::normalize(cameraFront * pitchmat);
	//pitchmat = RCQuaternion::rotate(cameraUp, -attackAngle);
	//Vector3 frontTurnLeft = RCVector3::normalize(cameraFront * pitchmat);

	//Graphic::GetInstance().DrawLine(myPos, myPos + frontRise * attackLength, CAMERA_ID::ENEMY_CAMERA, vector3(1, 0, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontFall * attackLength, CAMERA_ID::ENEMY_CAMERA, vector3(1, 0, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontTurnRight * attackLength, CAMERA_ID::ENEMY_CAMERA, vector3(1, 0, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontTurnLeft * attackLength, CAMERA_ID::ENEMY_CAMERA, vector3(1, 0, 0));
}

void FirstAI::InputCurlOn()
{
	player._Get()->SetInputCurlOn(true);
}

void FirstAI::InputCurlOff()
{
	player._Get()->SetInputCurlOff(true);
}

void FirstAI::InputJump()
{
	player._Get()->SetInputJump(true);
}

void FirstAI::InputStep()
{
	player._Get()->SetInputStep(true);
}

void FirstAI::InputShotThread()
{
	player._Get()->SetInputShot(true);
}

void FirstAI::InputRewind()
{
	if (!player._Get()->ReturnThread().expired())//thread(weak_ptr)��empty�łȂ���Ί������s
		action->ChangeAction(ACTION_ID::REWIND_ACTION);
}

void FirstAI::InputStepCurl()
{
	InputStep();
	InputCurlOn();
}

void FirstAI::FinalInput()
{
	//�ŏI�I�ȓ��͌���(���g�̑O�㍶�E)��n���B�ړ��̌v�Z�͂��̌�v���C���[�N���X�ɔC����B
	myParam->vec = inputVec;
	//�ŏI�I�ȓ��͌���(�J����)��n���B�J�����̕����v�Z�͂��̌�J�����N���X�ɔC����B
	camera->AddInputAngleHV(inputAngle.x, inputAngle.y);
}