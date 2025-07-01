#pragma once
#include "kamataengine.h"
class Player;
class CameraController {
public:
	void Initialize();

	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();
	const KamataEngine::Camera& GetViewProjection() const { return camera_; }
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 1.0f;
		float top = 1.0f;
	};
	Rect movableArea_ = {0, 100, 0, 100};
	void setMovableArea(CameraController::Rect area) { movableArea_ = area; }
	KamataEngine::Vector3 targetPosition_;
	static inline const float kInterpolationRate = 0.1f;
	static inline const float kVelocityBias = 10.0f;
	static inline const Rect margin = {-9.0f,9.0f,5.0f,-5.0f};
	

private:
	KamataEngine::Camera camera_;
	Player* target_ = nullptr;
	KamataEngine::Vector3 targetOffset_ = {0, 0, -15.0f};
	
};
