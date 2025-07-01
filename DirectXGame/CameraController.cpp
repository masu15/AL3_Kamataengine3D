#define NOMINMAX
#include "CameraController.h"
#include"Player.h"
#include"math.h"
#include <algorithm>
using namespace KamataEngine;
using namespace MathUtility;
void CameraController::Initialize() {
	camera_.Initialize();
	
	
}
void CameraController::Update() 
{
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	targetPosition_ = targetWorldTransform.translation_ + targetOffset_+targetVelocity*kVelocityBias;
	camera_.translation_.x = Lerp(camera_.translation_.x, targetPosition_.x, kInterpolationRate);
	
	camera_.translation_.x = std::max(camera_.translation_.x, movableArea_.left);
	camera_.translation_.x = std::min(camera_.translation_.x, movableArea_.right);
	camera_.translation_.y = std::max(camera_.translation_.y, movableArea_.bottom);
	camera_.translation_.y = std::min(camera_.translation_.y, movableArea_.top);
	camera_.UpdateMatrix();
}
void CameraController::Reset() { 
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;
}
