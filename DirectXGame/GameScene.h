#pragma once
#include"KamataEngine.h"
#include"player.h"
class GameScene
{
public:
	void Initialize();

	void Update();

	void Draw();

~GameScene();

    Player* player_ = nullptr;

private:
uint32_t textureHandle_ = 0;
KamataEngine::Model* model_ = nullptr;
KamataEngine::WorldTransform worldTransform_;
KamataEngine::Camera camera_;
};


