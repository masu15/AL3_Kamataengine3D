#include "GameScene.h"
#include"MyMath.h"
using namespace KamataEngine;

void GameScene::Initialize() { 
	textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = Model::Create();
	worldTransform_.Initialize();
	camera_.Initialize();
	player_ = new Player();
	modelBlock_ = Model::CreateFromOBJ("block");
	model_ = KamataEngine::Model::CreateFromOBJ("player");
	player_->Initialize(model_,&camera_);
	skydome_ = new skydome;
	modelskydome_ = KamataEngine::Model::CreateFromOBJ("SkyDome", true);
	skydome_->Initialize(modelskydome_, textureHandle_, &camera_);
	const uint32_t KNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	const float kBlockWidth = 2.0f;
	const float KBlockHeight = 2.0f;
	debugCamera_ = new DebugCamera(1280, 720);
	WorldTransformBlocks_.resize(KNumBlockVirtical);
	for (uint32_t i = 0; i < KNumBlockVirtical; i++) {
		WorldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	for (uint32_t i = 0; i < KNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if ((i+j) %2 == 0)
			{
				continue;
			}
			WorldTransformBlocks_[i][j] = new WorldTransform();
			WorldTransformBlocks_[i][j]->Initialize();
			WorldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			WorldTransformBlocks_[i][j]->translation_.y = KBlockHeight * i;
		}
	}
}

void GameScene::Update() {
	player_->Update();
	debugCamera_->Update();
	skydome_->Update();
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
	}
	for (std::vector<WorldTransform*>& worldTransformBlockLine : WorldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
				// アフィン変換の作成
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				worldTransformBlock->TransferMatrix();
			   
		}
	}
}

void GameScene::Draw() 
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance(); 

	Model::PreDraw(dxCommon->GetCommandList());

	//model_->Draw(worldTransform_, camera_, textureHandle_);

	player_->Draw();
	
	skydome_->Draw();
	for (std::vector<WorldTransform*>& worldTransformBlockLine : WorldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
				modelBlock_->Draw(*worldTransformBlock, camera_);
			
		}
	}
	Model::PostDraw();
}
GameScene::~GameScene() 
{ 
	delete model_;
	delete player_;
	delete modelskydome_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : WorldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
			
		}

	}
	delete debugCamera_;
}
