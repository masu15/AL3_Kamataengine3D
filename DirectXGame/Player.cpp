#include "Player.h"
#include"cassert"
using namespace KamataEngine;
void Player::Initialize(Model* model,Camera*camera) { 
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	camera_ = camera;
}

void Player::Update() {};

void Player::Draw() 
{ model_->Draw(worldTransform_, *camera_); }
