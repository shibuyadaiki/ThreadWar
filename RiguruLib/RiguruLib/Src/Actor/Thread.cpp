#include "Thread.h"
#include  "../Other/Device.h"
#include "../Math/Vector2.h"
#include "../Math/MathUtility.h"
#include "../Graphic/Graphic.h"
#include "Collision.h"
#include "ID.h"
#include "Player.h"
#include "Stage.h"
#include "ThreadWeb.h"
#include "../Math/Converter.h"
#include "ThreadBullet.h"

//�����L�т鑬�x
const float THREAD_SPEED = 120;

Thread::Thread(IWorld& world_, std::weak_ptr<Player> player_, std::weak_ptr<Stage> stage_, CAMERA_ID cID,bool startPosGroundTouch_) 
:Actor(world_),
player(player_),
stage(stage_){
	//�e��p�����[�^�[�ɒl���Z�b�g
	parameter.isDead = false;
	threadParam.startPosition = RCMatrix4::getPosition(*player._Get()->ReturnMat());
	threadParam.currentEndPosition = threadParam.startPosition;
	threadParam.startPosIsGroundTouch = startPosGroundTouch_;
	isRewind = false;
	isGoingEnd = false;
	goingVec = vector3(0, 0, 0);
	rewindSpeed = 12.0f;
	isShot = true;
	shotSpeed = THREAD_SPEED;
	threadHit = false;
	rootActor = player._Get()->GetParameter().id;
	if (rootActor == ACTOR_ID::PLAYER_ACTOR) parameter.id = ACTOR_ID::PLAYER_THREAD_ACTOR;
	else parameter.id = ACTOR_ID::ENEMY_THREAD_ACTOR;

	//���̔���
	Vector3 eye = Device::GetInstance().GetCamera(cID)->CameraParam()->Eye;
	Vector3 target = Device::GetInstance().GetCamera(cID)->CameraParam()->Target;
	Vector3 vec =RCVector3::normalize(target - eye);
	//�J�������烂�f���ւ̃q�b�g�n�_�֌���������
	CollisionParameter col = ModelRay(*stage._Get()->ReturnMat(), OCT_ID::STAGE_OCT, eye,eye + vec * 10000.0f);
	threadParam.endPosition = col.colPos;
	if (col.colFlag){
		////�v���C���[����q�b�g�n�_�֌���������
		//CollisionParameter col_ = ModelRay(*stage._Get()->ReturnMat(), OCT_ID::STAGE_OCT, threadParam.startPosition + RCVector3::normalize(RCMatrix4::getUp(*player._Get()->ReturnMat())) * 0.1f, col.colPos);
		//
		//threadParam.endPosition = col_.colPos;

	}
	else{
		parameter.isDead = true;
	}
		threadParam.startNormal = RCVector3::normalize(threadParam.startPosition - threadParam.endPosition);
		threadParam.endNormal = RCVector3::normalize(threadParam.endPosition - threadParam.startPosition);
		dangle = false;

	selectThread = false;

	AddThreadWebInit();

}

Thread::~Thread(){

}

void Thread::Initialize(){
	world.Add(parameter.id == ACTOR_ID::PLAYER_THREAD_ACTOR ? ACTOR_ID::PLAYER_THREADBULLET_ACTOR : ACTOR_ID::ENEMY_THREADBULLET_ACTOR,
		std::make_shared<ThreadBullet>(world, stage, parameter.id, threadParam.startPosition, threadParam.currentEndPosition, threadParam.endPosition, shared_from_this(), cID));
}

void Thread::Update(float frameTime){
	//�n�_�ƏI�_�̋��������ȉ����A�V���b�g���łȂ���Ύ��S
	if (RCVector3::length(threadParam.startPosition - threadParam.currentEndPosition) < 0.3f && !IsShot())
		parameter.isDead = true;

	//�����ˏ���
	if (IsShot()){
		ThreadShot(frameTime);
		//shotSpeed = THREAD_SPEED;
		//�����蔻��̃Z�b�g
		world.SetCollideSelect(this->shared_from_this(),parameter.id == ACTOR_ID::PLAYER_THREAD_ACTOR ? ACTOR_ID::ENEMY_THREADWEB_ACTOR : ACTOR_ID::PLAYER_THREADWEB_ACTOR, COL_ID::TRIANGLE_LINE_T_COLL);
	}


	CollisionParameter col_;
	if (!IsShot()){
		col_ = ModelRay(*stage._Get()->ReturnMat(), OCT_ID::STAGE_OCT, threadParam.startPosition + threadParam.endNormal * 0.1f, threadParam.endPosition + threadParam.startNormal * 0.1f);
		if (col_.colFlag){
			threadParam.endPosition = col_.colPos;
			threadParam.currentEndPosition = col_.colPos;
		}
	}
	if (dangle){
		threadParam.startNormal = RCVector3::normalize(threadParam.startPosition - threadParam.endPosition);
		threadParam.endNormal = RCVector3::normalize(threadParam.endPosition - threadParam.startPosition);
	}
	else{
		col_ = ModelRay(*stage._Get()->ReturnMat(), OCT_ID::STAGE_OCT, threadParam.endPosition + threadParam.startNormal * 0.1f, threadParam.startPosition + threadParam.endNormal * 0.1f);
		if (col_.colFlag){
			threadParam.startPosition = col_.colPos;
		}
	}
	color = vector3(1, 0, 0);
	if (parameter.id == ACTOR_ID::ENEMY_THREAD_ACTOR)color = vector3(0, 0, 1);
	if (selectThread)color = vector3(1, 1, 1);
	selectThread = false;

	if (RCVector3::length(threadParam.startPosition - threadParam.endPosition) < 2.0f && !dangle){
		parameter.isDead = true;
	}
}

void Thread::Draw(CAMERA_ID cID) const{
	Graphic::GetInstance().DrawLine(threadParam.startPosition, threadParam.currentEndPosition, cID,color);
}

void Thread::OnCollide(Actor& other, CollisionParameter colpara){
	if (colpara.id == COL_ID::TRIANGLE_LINE_T_COLL ){
		//�V���b�g�̃X�s�[�h�𔼌��B
		parameter.isDead = true;
		static_cast<ThreadWeb*>(&other)->PlusHitCount(1);
		threadHit = true;
	}
}

ThreadParameter Thread::GetThreadParameter(){
	return threadParam;
}
//�������J�n
void Thread::StartRewind(){
	//������蒆�ƃV���b�g���̓X���[�B
	if (IsRewind() || IsShot()) return;
	isRewind = true;

	//�v���C���[�̌������A�n�_�ƏI�_�̂ǂ���ɂ������Ă��邩���r
	Vector3 pFrontNor = RCVector3::normalize(-RCMatrix4::getFront(player._Get()->GetParameter().matrix));
	float deg_startNor = Math::acos(RCVector3::dot(pFrontNor, threadParam.startNormal));
	float deg_endNor = Math::acos(RCVector3::dot(pFrontNor, threadParam.endNormal));

	if (deg_endNor < deg_startNor) isGoingEnd = true;
	else isGoingEnd = false;

	Vector3 init = RCMatrix4::getPosition(player._Get()->GetParameter().matrix) - goingVec;
	//�v���C���[�̈ʒu����X�^�[�g
	if (isGoingEnd){
		goingVec = threadParam.endNormal;
		threadParam.startPosition = init;
	}
	else{
		goingVec = threadParam.startNormal;
		threadParam.endPosition = init;
	}
}
//�������
void Thread::ThreadRewind(float frameTime){
	//��������Ă���ʒu�ƃv���C���[�̈ʒu���ړ�
	if (isGoingEnd){
		threadParam.startPosition += goingVec * rewindSpeed * frameTime;
		player._Get()->SetPos(threadParam.startPosition);
	}
	else{
		threadParam.endPosition += goingVec * rewindSpeed * frameTime;
		player._Get()->SetPos(threadParam.endPosition);
	}
	//�v���C���[�̈ړ������ɃZ�b�g
	player._Get()->SetMoveVec(goingVec * rewindSpeed * frameTime);
}
//����蒆���ǂ���
bool Thread::IsRewind()
{
	return isRewind;
}

void Thread::AddThreadWebInit(){
	//�ŏI�I�ɐ�������Ƃ��Ɏg����
	int count = 0;

	//���[���h��ɑ��݂��邷�ׂĂ̎�������
	world.EachActor(parameter.id, [&](const Actor& other){
		Thread* otherT = static_cast<Thread*>(const_cast<Actor*>(&other));
		if (this != otherT){
			ACTOR_ID a = otherT->GetParameter().id;
			if (parameter.id == otherT->GetParameter().id){
				ThreadParameter otherParam = otherT->GetThreadParameter();

				//�����m�̂Ȃ��p�x�����߂�
				float angle = Math::acos(RCVector3::dot(threadParam.endNormal, otherParam.endNormal));
				//�p�x�����l�ȉ��Ȃ画��J�n
				if (angle <= 45.0f && angle >= 10.0f){
					CollisionParameter result = IsCollideLineToSphere(otherParam.startPosition, otherParam.endPosition, RCMatrix4::getPosition(player._Get()->GetParameter().matrix), 0.5f);
					if (result.colFlag && count < 2){
						threads.push_back(otherT->shared_from_this());
						count++;
					}
				}
			}
		}
	});
}
//�w偂̑��̐���
void Thread::AddThreadWeb(){
	ACTOR_ID webID;
	if (parameter.id == ACTOR_ID::PLAYER_THREAD_ACTOR) webID = ACTOR_ID::PLAYER_THREADWEB_ACTOR;
	else webID = ACTOR_ID::ENEMY_THREADWEB_ACTOR;
	for (auto& i : threads){
		if (!i._Get()->IsDead())
		world.Add(webID, std::make_shared<ThreadWeb>(world, this->shared_from_this(), i._Get()->shared_from_this()));
	}
}
//����蒆�̈ړ������x�N�g�����擾�i�P�ʃx�N�g���j
Vector3 Thread::GetRewindGoingVector()
{
	return goingVec;
}

ACTOR_ID Thread::GetRootActor(){
	return player._Get()->GetParameter().id;
}

void Thread::ThreadShot(float frameTime){
	//����L�΂��B
	threadParam.currentEndPosition += threadParam.endNormal * shotSpeed * frameTime;
	//���݂̎��̒��������߂�B
	float curDis = RCVector3::distance_(threadParam.startPosition, threadParam.currentEndPosition);
	//�ŏI�I�Ȏ��̒��������߂�B
	endDis = RCVector3::distance_(threadParam.startPosition, threadParam.endPosition);
	//�ŏI�I�Ȏ��̒����ƌ��݂̎��̒������r
	if (endDis <= curDis){
		threadParam.currentEndPosition = threadParam.endPosition;
		isShot = false;

		//�w偂̑��̐���
		AddThreadWeb();
	}
}

bool Thread::IsShot(){
	return isShot;
}

void Thread::SetDangleFlag(bool dangle_){
	dangle = dangle_;
}

bool Thread::ReturnDangleFlag(){
	return dangle;
}

void Thread::SetIsDead(bool isDead_){
	parameter.isDead = isDead_;
}

void Thread::SetStartPos(Vector3 endPos_){
	threadParam.startPosition = endPos_;
}

void Thread::SetEndPos(Vector3 endPos_){
	threadParam.currentEndPosition = endPos_;
}