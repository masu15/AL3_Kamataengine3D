#pragma once
#include"KamataEngine.h"
class skydome {
public:
	void Initialize();
	void Update();
	void Draw();

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model = nullptr;
};
