#pragma once
#include"kamataengine.h"
class Player;
class CameraController 
{
public:
	void Initialize();

	void Update();

private:
	KamataEngine::Camera camera_;
	Player* target = nullptr;
};
