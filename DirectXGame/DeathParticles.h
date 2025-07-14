#pragma once
#include <KamataEngine.h>
#include<array>
#include <numbers>
class DeathParticles 
{
public:
	void Initialize(KamataEngine::Model* mod, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	void Update();

	void Draw();

	static inline const uint32_t kNumParticles = 8;
	static inline const float kDuration = 3.0f;
	static inline const float kSpeed = 0.1f;
	static inline const float kAngleUnit = (2 * std::numbers::pi_v<float> / kNumParticles);
	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_;
	bool isFinished_ = false;
	float counter_ = 0.0f;
	
	KamataEngine::ObjectColor objectColor_;
	KamataEngine::Vector4 color_;

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	
};
