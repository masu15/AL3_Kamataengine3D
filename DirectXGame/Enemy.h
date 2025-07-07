#pragma once
#include"KamataEngine.h"
#include "MyMath.h"
class Player;
class Enemy {
public:

	void Initialize(KamataEngine::Model* model,KamataEngine::Camera* camera,KamataEngine::Vector3& position);

	void Update();

	void Draw();

	AABB GetAABB();

	KamataEngine::Vector3 GetWorldPosition();

	void OnCollision(const Player* player);

	static inline const float kWalkSpeed = 0.01f;
	KamataEngine::Vector3 velocity_ = {};

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	float walkTimer = 0.0f;
	static inline const float KWidth = 0.8f;
	static inline const float KHeight = 0.8f;
};
