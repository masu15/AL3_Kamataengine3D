#include "Enemy.h"
#include"numbers"
#include "MyMath.h"
#include <algorithm>
#include <cassert>
using namespace KamataEngine;
using namespace MathUtility;
void Enemy::Initialize(Model* model, Camera* camera, Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;
	camera_ = camera;
	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer = 0.0f;
}
void Enemy::Update() {
	worldTransform_.translation_ += velocity_;
	walkTimer += 10.0f / 60.0f;
	worldTransform_.rotation_.x = std::sin(walkTimer/2);
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}
void Enemy::Draw()
{ 
	model_->Draw(worldTransform_, *camera_);
}
