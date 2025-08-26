#pragma once
#include <KamataEngine.h>
#include"Fade.h"


class TitleScene 
{
	public:
	enum class Phase {
		KFadeIn,
		KMain,
		KFadeOut,
	};
	void Initialize();

	void Update();

	void Draw();

	~TitleScene();
	
	bool ISFinished() const { return finished_; }

private:
	bool finished_ = false;
	KamataEngine::Camera camera_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;
	float rotate = 0.0f;
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::KFadeIn;
};
