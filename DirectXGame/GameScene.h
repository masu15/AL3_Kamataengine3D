#pragma once
#include"KamataEngine.h"
#include"player.h"
#include<vector>
class GameScene
{
public:
	void Initialize();

	void Update();

	void Draw();

~GameScene();

    Player* player_ = nullptr;
std::vector<std::vector<KamataEngine::WorldTransform*>> WorldTransformBlocks_;
private:
uint32_t textureHandle_ = 0;
KamataEngine::Model* model_ = nullptr;
KamataEngine::WorldTransform worldTransform_;
KamataEngine::Camera camera_;
KamataEngine::Model* modelBlock_ = nullptr;
bool isDebugCameraActive_ = false;
KamataEngine::DebugCamera* debugCamera_ = nullptr;
};


