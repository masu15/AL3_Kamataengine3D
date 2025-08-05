#include "TitleScene.h"

using namespace KamataEngine;
void TitleScene::Initialize() 
{
	model_ = Model::CreateFromOBJ("titleFont");
	modelPlayer_ = Model::CreateFromOBJ("player");

	camera_.Initialize();

	worldTransform_.Initialize();
	worldTransformPlayer_.Initialize();
	fade_ = new Fade;
	fade_->Initialize();
	fade_->Start(Fade::)
}
void TitleScene::Update() {
	switch (phase_)
	{ 
		case Phase::KMain:
		
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
