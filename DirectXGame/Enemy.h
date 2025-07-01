#pragma once
#include"KamataEngine.h"

class Enemy {
public:

	void Initialize(KamataEngine::Model* model,KamataEngine::Camera* camera,KamataEngine::Vector3& position);

	void Update();

	void Draw();

	static inline const float kWalkSpeed = 0.01f;
	KamataEngine::Vector3 velocity_ = {};

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	float walkTimer = 0.0f;
	
};
