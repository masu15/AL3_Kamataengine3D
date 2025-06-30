#pragma once
#include"KamataEngine.h"
#include"player.h"
#include"skydome.h"
#include"MapChipField.h"
#include"CameraController.h"
#include"Enemy.h"
#include<vector>
class GameScene
{
public:
	void Initialize();

	void Update();

	void Draw();

	void GenerateBlocks();

	~GameScene();

    Player* player_ = nullptr;
    skydome* skydome_ = nullptr;
	Enemy* enemy_ = nullptr;
	MapChipField* mapChipField_;
	CameraController* cameraController_;
	std::vector<std::vector<KamataEngine::WorldTransform*>> WorldTransformBlocks_;
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
};


