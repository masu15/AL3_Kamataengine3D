#include "GameScene.h"
#include"MyMath.h"
using namespace KamataEngine;

void GameScene::Initialize() { 
	textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = Model::Create();
	worldTransform_.Initialize();
	camera_.Initialize();
	modelBlock_ = Model::CreateFromOBJ("block");
	model_ = KamataEngine::Model::CreateFromOBJ("player");
	modelEnemy_ = KamataEngine::Model::CreateFromOBJ("enemy");
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	skydome_ = new skydome;
	modelskydome_ = KamataEngine::Model::CreateFromOBJ("SkyDome", true);
	skydome_->Initialize(modelskydome_, textureHandle_, &camera_);
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	player_ = new Player();
	player_->Initialize(model_, &camera_, playerPosition);
	player_->SetMapChipField(mapChipField_);
	cameraController_ = new CameraController;
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->setMovableArea(cameraArea);
	GenerateBlocks();
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(15, 18);
	enemy_ = new Enemy;
	enemy_->Initialize(modelEnemy_, &camera_,enemyPosition);

	/*player_->Initialize(model_, &camera_, playerPosition);*/
	//const float kBlockWidth = 2.0f;
	//const float KBlockHeight = 2.0f;
	debugCamera_ = new DebugCamera(1280, 720);
	//WorldTransformBlocks_.resize(KNumBlockVirtical);
	//for (uint32_t i = 0; i < KNumBlockVirtical; i++) {
	//	WorldTransformBlocks_[i].resize(kNumBlockHorizontal);
	//}
	//for (uint32_t i = 0; i < KNumBlockVirtical; ++i) {
	//	for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
	//		if ((i+j) %2 == 0)
	//		{
	//			continue;
	//		}
	//		WorldTransformBlocks_[i][j] = new WorldTransform();
	//		WorldTransformBlocks_[i][j]->Initialize();
	//		WorldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
	//		WorldTransformBlocks_[i][j]->translation_.y = KBlockHeight * i;
	//	}
	//}
}

void GameScene::Update() {
	player_->Update();
	debugCamera_->Update();
	skydome_->Update();
	cameraController_->Update();
	enemy_->Update();
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
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		camera_.TransferMatrix();
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

	model_->Draw(worldTransform_, camera_, textureHandle_);

	player_->Draw();
	
	skydome_->Draw();

	enemy_->Draw();
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

void GameScene::GenerateBlocks()
{
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetkNumBlockHorizontal();
	WorldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		WorldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		for (uint32_t j = 0; j < numBlockHorizontal; j++) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				WorldTransformBlocks_[i][j] = worldTransform;
				WorldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			   
			}

		}
	}
}


GameScene::~GameScene() 
{ 
	delete model_;
	delete player_;
	delete modelskydome_;
	delete enemy_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : WorldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
			
		}

	}
	delete debugCamera_;
}
