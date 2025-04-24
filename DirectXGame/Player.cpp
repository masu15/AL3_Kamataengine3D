#include "Player.h"
#include"cassert"
using namespace KamataEngine;
void Player::Initialize(Model* model, uint32_t texturehandle,Camera*camera) { 
	assert(model);
	model_ = model;
	textureHandle_=texturehandle;
	worldTransform_.Initialize();
	camera_ = camera;
}

void Player::Update() {};

void Player::Draw() 
{ model_->Draw(worldTransform_, *camera_, textureHandle_); }
