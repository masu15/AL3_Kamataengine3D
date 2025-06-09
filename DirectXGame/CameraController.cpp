#include "CameraController.h"
#include"Player.h"
using namespace KamataEngine;
void CameraController::Initialize() {
	camera_.Initialize();
}
void CameraController::Update() {

}

void CameraController::Reset() { 
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;
	camera_.UpdateMatrix();
}
