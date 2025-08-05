#include "GameScene.h"
#include "MyMath.h"

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
	deathParticles_ = new DeathParticles;
	modelDeathParticles_ = KamataEngine::Model::CreateFromOBJ("deathParticle");
	Vector3 DeathPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	deathParticles_->Initialize(modelDeathParticles_, &camera_, DeathPosition);
	phase_ = Phase::kplay;
	for (int32_t i = 0; i < 3; i++) {
		Enemy* newEnemy = new Enemy;
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(i * 10, 18);
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);
		enemies_.push_back(newEnemy);
	}
	/*player_->Initialize(model_, &camera_, playerPosition);*/
	// const float kBlockWidth = 2.0f;
	// const float KBlockHeight = 2.0f;
	debugCamera_ = new DebugCamera(1280, 720);
	// WorldTransformBlocks_.resize(KNumBlockVirtical);
	// for (uint32_t i = 0; i < KNumBlockVirtical; i++) {
	//	WorldTransformBlocks_[i].resize(kNumBlockHorizontal);
	// }
	// for (uint32_t i = 0; i < KNumBlockVirtical; ++i) {
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
	// }
}

void GameScene::Update() {
	switch (phase_) {
	case Phase::kplay:
		skydome_->Update();
		player_->Update();
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
		cameraController_->Update();
		debugCamera_->Update();
		CheckAllCollisions();
		ChangePhase();
		break;
	case Phase::kDeath:
		skydome_->Update();
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
		if (deathParticles_) {
			deathParticles_->Update();
		}
		debugCamera_->Update();
		ChangePhase();
		break;
		
	}
	
	
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

void GameScene::CheckAllCollisions() {
#pragma region PlayerEnemyHit
	AABB aabb1, aabb2;
	aabb1 = player_->GetAABB();

	for (Enemy* enemy : enemies_) {
		aabb2 = enemy->GetAABB();
		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(enemy);
			enemy->OnCollision(player_);
		}
	}

#pragma endregion
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());
	switch (phase_) {
	case Phase::kplay:
		

		player_->Draw();
		break;
	case Phase::kDeath:

		if (deathParticles_) {
			deathParticles_->Draw();
		}
		break;
	}
	model_->Draw(worldTransform_, camera_, textureHandle_);
	skydome_->Draw();
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}
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

void GameScene::GenerateBlocks() {
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

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete modelskydome_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (std::vector<WorldTransform*>& worldTransformBlockLine : WorldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	delete debugCamera_;
	delete modelDeathParticles_;
}

void GameScene::ChangePhase()
{
	switch (phase_) {
	case Phase::kplay:
		if (player_->IsDead()) {
			phase_ = Phase::kDeath;
			Vector3 deathParticlesPosition = player_->GetWorldPosition();
			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(modelDeathParticles_, &camera_,deathParticlesPosition );
		}
		break;
	case Phase::kDeath:
		if (deathParticles_ && deathParticles_->ISFinished()){
			finished_ = true;
		}
		break;
	}
}
