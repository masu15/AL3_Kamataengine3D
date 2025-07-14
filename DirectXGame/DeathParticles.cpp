#include "DeathParticles.h"
#include "MyMath.h"
#include <algorithm>
using namespace KamataEngine;
using namespace MathUtility;
void DeathParticles::Initialize(Model* model, Camera* camera, Vector3& position) {
	assert(model);
	model_ = model;
	camera_ = camera;
	for (WorldTransform& worldTransform :worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}
void DeathParticles::Update() {
	if (isFinished_) {
		return;
	}
	for (uint32_t i = 0; i < kNumParticles; ++i) {
		Vector3 velocity = {kSpeed, 0, 0};
		float angle = kAngleUnit * i;
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		velocity = Transform(velocity, matrixRotation);
		worldTransforms_[i].translation_ += velocity;
	}
	counter_ += 1.0f / 60.0f;
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		isFinished_ = true;
	}
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
		worldTransform.TransferMatrix();
	}
	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	objectColor_.SetColor(color_);
}
void DeathParticles::Draw() { 
	if (isFinished_) {
		return;
	}
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *camera_,&objectColor_);
	}
}