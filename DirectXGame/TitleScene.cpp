#include "TitleScene.h"
#include"MyMath.h"
using namespace KamataEngine;
void TitleScene::Initialize() 
{
	model_ = Model::CreateFromOBJ("titleFont");
	modelPlayer_ = Model::CreateFromOBJ("player");

	camera_.Initialize();

	worldTransform_.Initialize();
	worldTransform_.scale_ = {2, 2, 2};
	worldTransform_.translation_ = {0, 8, 0};

	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {10, 10, 10};
	worldTransformPlayer_.translation_ = {0, -8, 0};
	worldTransformPlayer_.rotation_.y = 3.14f;
	fade_ = new Fade;
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn,1.0f);
}
void TitleScene::Update() {
	switch (phase_)
	{ 
		case Phase::KMain:
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			    phase_ = Phase::KFadeOut;
			fade_->Start(Fade::Status::FadeOut,1.0f);
		}
		break;
	    case Phase::KFadeIn:
		    fade_->Update();
		    if (fade_->isFinished()) {
			  phase_=  Phase::KMain;
			}
		    break;
	    case Phase::KFadeOut:
		    fade_->Update();
		    if (fade_->isFinished()) {
			    finished_ = true;
			    break;
			}
		    worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.rotation_);
		    rotate += 0.1f;
		    worldTransformPlayer_.rotation_.y = 3.14f + sin(rotate);
	}
	

if (KamataEngine::Input::GetInstance()->PushKey(DIK_SPACE)) {
	}
}
void TitleScene::Draw() 
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());
	
	model_->Draw(worldTransform_, camera_ );
	modelPlayer_->Draw(worldTransformPlayer_, camera_);
	Model::PostDraw();
	fade_->Draw();
}

TitleScene::~TitleScene() 
{ 
	delete model_;
	delete modelPlayer_;
	delete fade_;
}
