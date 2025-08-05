#pragma once
#include"KamataEngine.h"
#include"player.h"
#include"skydome.h"
#include"MapChipField.h"
#include"CameraController.h"
#include"Enemy.h"
#include"DeathParticles.h"
#include<vector>
#include "Fade.h"
enum class Phase 
{
	kFadeIn,
	kplay,
	kDeath,
	kFadeOut
};
class GameScene
{
public:
	void Initialize();

	void Update();

	void CheckAllCollisions();

	void Draw();

	void GenerateBlocks();

	~GameScene();

	void ChangePhase();

    Player* player_ = nullptr;
    skydome* skydome_ = nullptr;
	std::list<Enemy*> enemies_;
	MapChipField* mapChipField_;
	CameraController* cameraController_;

	std::vector<std::vector<KamataEngine::WorldTransform*>> WorldTransformBlocks_;

	bool finished_ = false;
	bool ISFinished() const { return finished_; }

private:
uint32_t textureHandle_ = 0;
KamataEngine::Model* model_ = nullptr;
KamataEngine::WorldTransform worldTransform_;
KamataEngine::Camera camera_;
KamataEngine::Model* modelBlock_ = nullptr;
bool isDebugCameraActive_ = false;
KamataEngine::DebugCamera* debugCamera_ = nullptr;
KamataEngine::Model* modelskydome_ = nullptr;
KamataEngine::Model* modelEnemy_ = nullptr;
KamataEngine::Model* modelDeathParticles_;
DeathParticles* deathParticles_ = nullptr;
Phase phase_;
};


