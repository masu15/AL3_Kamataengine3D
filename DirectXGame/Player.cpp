#define NOMINMAX
#include "Player.h"
#include"cassert"
#include"numbers"
#include "MyMath.h"
#include <algorithm>
using namespace KamataEngine;
using namespace MathUtility;
void Player::Initialize(Model* model, Camera* camera,Vector3& position) { 
	
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	camera_ = camera;
}

void Player::Update() 
{ 
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - KAttenuation);
				}
				acceleration.x += KAcceleration;
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = KTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - KAttenuation);
				}
				acceleration.x -= KAcceleration;
				if (lrDirection_ != LRDirection::KLeft) {
					lrDirection_ = LRDirection::KLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = KTimeTurn;
				}
			}
			velocity_ += acceleration;
			velocity_.x = std::clamp(velocity_.x, -KLimitRunSpeed, KLimitRunSpeed);
		} else {
			velocity_.x *= (1.0f - KAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_ += Vector3(0, KJumpAcceleration, 0);
		}
	} else {
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
	
	bool landing = false;

	if (velocity_.y < 0) {
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	} else {
		if (landing) {
			worldTransform_.translation_.y = 1.0f;
			velocity_.x *= (1.0f - KAcceleration);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}
	worldTransform_.translation_ += velocity_;
	
	if (turnTimer_ > 0.0f) 
	{
		turnTimer_ -= 1.0f / 60.0f;
		float destinationRotationYTable[] ={
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> * 3.0f / 2.0f
		};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / KTimeTurn);
	}
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Player::Draw() 
{ model_->Draw(worldTransform_, *camera_); }
