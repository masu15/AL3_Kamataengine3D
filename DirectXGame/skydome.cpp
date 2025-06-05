#include "skydome.h"
#include"cassert"
using namespace KamataEngine;
void skydome::Initialize(KamataEngine::Model* model, uint32_t texturehandle, KamataEngine::Camera* camera) {
	assert(model);
	model_ = model;
	textureHandle_ = texturehandle;
	worldTransform_.Initialize();
	camera_ = camera;
}

void skydome::Update() {}

void skydome::Draw() 
{ 
	 model_->Draw(worldTransform_,*camera_); 
}



