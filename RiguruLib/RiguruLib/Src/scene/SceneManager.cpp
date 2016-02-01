#define NOMINMAX
#include"SceneManager.h"
#include"IScene.h"
#include <algorithm>

const int SceneManager::MaxStageCount = 6;

//�R���X�g���N�^
SceneManager::SceneManager() :
	mStageCount(1){
}

//�X�V�O������
void SceneManager::Initialize(){
	End();
	mScenes.clear();
}

//�X�V
void SceneManager::Update(float frameTime){
	mCurrentScene->Update(frameTime);
}

//�`��
void SceneManager::Draw() const{
	mCurrentScene->Draw();
}

//�I��
void SceneManager::End(){
	mCurrentScene->End();
}

void SceneManager::Change()
{
	if (mCurrentScene->IsEnd())
	{
		Change(mCurrentScene->Next());
	}
}

//�V�[���̒ǉ�
void SceneManager::Add(Scene name, const IScenePtr& scene){
	mScenes[name] = scene;
}

void SceneManager::SetScene(Scene name){
	mCurrentScene = mScenes[name];
	mCurrentScene->Initialize();
}

//�V�[���̕ύX
void SceneManager::Change(Scene name){
	Scene now = Scene::None;
	for (auto& i : mScenes)
	{
		if (mCurrentScene == i.second)
			now = i.first;
	}
	if (name == now)
		return;
	
	End();
	mCurrentScene = mScenes[name];
	mCurrentScene->Initialize();
}

// ���������w�肷��
void SceneManager::Init(Scene name)
{
	mScenes.at(name)->Initialize();
}

// �I���������w�肷��
void SceneManager::Final(Scene name)
{
	mScenes.at(name)->End();
}

static int _Clamp(int t, int min, int max)
{
	return std::min(max, std::max(t, min));
}

void SceneManager::SetStageCount(int n)
{
	mStageCount = _Clamp(n, 0, SceneManager::MaxStageCount);
}