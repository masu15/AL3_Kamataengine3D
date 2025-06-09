#pragma once
#include "KamataEngine.h"
enum class LRDirection { 
	kRight,
	KLeft
};
class Player 
{
public:
	void Initialize(KamataEngine::Model* mod, KamataEngine::Camera * camera,KamataEngine::Vector3 &position);

	void Update(); 

	void Draw();
	static inline const float KAcceleration = 0.1f;
	static inline const float KAttenuation = 0.1f;
	static inline const float KLimitRunSpeed = 0.5f;
	static inline const float KTimeTurn = 0.3f;
	static inline const float kGravityAcceleration = 0.1f;
	static inline const float kLimitFallSpeed = 1.0f;
	static inline const float KJumpAcceleration = 1.0f;
	private:

	KamataEngine::WorldTransform worldTransform_;
    KamataEngine::Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::Vector3 velocity_ = {};
	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	bool onGround_ = true;
};
