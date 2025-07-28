#pragma once
#include <KamataEngine.h>
class TitleScene 
{
	public:
	void Initialize();

	void Update();

	void Draw();

	bool finished_ = false;
	bool ISFinished() const { return finished_;}
	KamataEngine::Camera camera_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;

private:
	
};
