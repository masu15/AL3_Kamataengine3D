#define NOMINMAX
#include "Player.h"
#include"numbers"
#include <algorithm>
#include"MapChipField.h"
using namespace KamataEngine;
using namespace MathUtility;
void Player::Initialize(Model* model, Camera* camera,Vector3& position) { 
	
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	camera_ = camera;
}

void Player::Update() 
{ 
	//移動入力
	InputMove();
	//移動量を加味して衝突判定する
	CollisionMapInfo collisionMapInfo;
	//移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;
	//マップ衝突チェック
	CheckMapCollision(collisionMapInfo);
	// 判定結果を反映して移動させる
	//移動
	CheckMapMove(collisionMapInfo);
	// 天井に接触している場合の処理
	CheckMapCeiling(collisionMapInfo);
	// 壁に接触している場合の処理
	CheckMapWall(collisionMapInfo);
	// 接地状況の切り替え
	CheckMapLanding(collisionMapInfo);
	////着地フラグ
	//bool landing = false;
	////地面との当たり判定
	////下降中？
	//if (velocity_.y < 0) {
	//	if (worldTransform_.translation_.y <= 1.0f) {
	//		landing = true;
	//	}
	//}
	////接地判定
	//if (onGround_) {
	//	if (velocity_.y > 0.0f) {
	//		onGround_ = false;
	//	}
	//} else {
	//	if (landing) {
	//		worldTransform_.translation_.y = 1.0f;
	//		velocity_.x *= (1.0f - KAcceleration);
	//		velocity_.y = 0.0f;
	//		onGround_ = true;
	//	}
	//}
	//旋回制御
	AnimateTurn();
	//行列計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Player::InputMove() 
{
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - KAttenuation);
				}
				acceleration.x += KAcceleration;
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = KTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - KAttenuation);
				}
				acceleration.x -= KAcceleration;
				if (lrDirection_ != LRDirection::KLeft) {
					lrDirection_ = LRDirection::KLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = KTimeTurn;
				}
			}
			velocity_ += acceleration;
			velocity_.x = std::clamp(velocity_.x, -KLimitRunSpeed, KLimitRunSpeed);
		} else {
			velocity_.x *= (1.0f - KAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_ += Vector3(0, KJumpAcceleration, 0);
		}
	} else {
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}

void Player::CheckMapCollision(CollisionMapInfo& info) 
{ 
	CheckMapCollisionUp(info); 
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

void Player::CheckMapMove(const CollisionMapInfo& info)
{ 
	worldTransform_.translation_ += info.move; 
}

void Player::CheckMapCeiling(const CollisionMapInfo& info)
{ 
	if (info.ceiling) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

void Player::CheckMapWall(const CollisionMapInfo& info) {
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}









void Player::AnimateTurn() {
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / KTimeTurn);
	}
}

void Player::Draw() 
{
	model_->Draw(worldTransform_, *camera_);
}

void Player::CheckMapCollisionUp(CollisionMapInfo& info) 
{
	if (info.move.y <= 0) {
		return;
	}
	std::array<KamataEngine::Vector3, KnumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	bool hit = false;
	MapChipField::IndexSet indexset;
	indexset = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[KLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexset.xIndex, indexset.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexset.xIndex, indexset.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	indexset = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexset.xIndex, indexset.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexset.xIndex, indexset.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		indexset = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +KHeight / 2.0f, 0));
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +KHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexset.yIndex) {
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexset.xIndex, indexset.yIndex);
			info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (KHeight / 2.0f + kBlank));
			info.ceiling = true;
		}
	}
}

void Player::CheckMapCollisionDown(CollisionMapInfo& info) {
	if (info.move.y >= 0) {
		return;
	}
	std::array<KamataEngine::Vector3, KnumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	bool hit = false;
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[KleftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -KHeight / 2.0f, 0));
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -KHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex) {
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (KHeight / 2.0f + kBlank));
			info.landing = true;
		}
	}
}
void Player::CheckMapCollisionRight(CollisionMapInfo& info) 
{
	if (info.move.x <= 0) {
		return;
	}
	std::array<KamataEngine::Vector3, KnumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	bool hit = false;
	MapChipField::IndexSet indexset;
	indexset = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexset.xIndex, indexset.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexset.xIndex-1, indexset.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	indexset = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexset.xIndex, indexset.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexset.xIndex-1, indexset.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		indexset = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3( +KWidth / 2.0f,0, 0));
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(+KHeight / 2.0f, 0, 0));
		if (indexSetNow.xIndex != indexset.xIndex) {
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexset.xIndex, indexset.yIndex);
			info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - (KWidth / 2.0f + kBlank));
			info.hitWall = true;
		}
	}
}
void Player::CheckMapCollisionLeft(CollisionMapInfo& info) 
{
	if (info.move.x >= 0) {
		return;
	}
	std::array<KamataEngine::Vector3, KnumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	bool hit = false;
	MapChipField::IndexSet indexset;
	indexset = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[KLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexset.xIndex, indexset.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexset.xIndex+1, indexset.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	indexset = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[KleftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexset.xIndex, indexset.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexset.xIndex+1, indexset.yIndex);
	if (mapChipType == MapChipType::kBlock&&mapChipTypeNext!=MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		indexset = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-KWidth / 2.0f, 0, 0));
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(-KHeight / 2.0f, 0, 0));
		if (indexSetNow.xIndex != indexset.xIndex) {
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexset.xIndex, indexset.yIndex);
			info.move.x = std::max(0.0f, rect.right - worldTransform_.translation_.x + (KWidth / 2.0f + kBlank));
			info.hitWall = true;
		}
	}
}
AABB Player::GetAABB() 
{   
	AABB aabb;


	return aabb;
}
Vector3 Player::GetWorldPosition() 
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
}
KamataEngine::Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner)
{ 
		KamataEngine::Vector3 offsetTable[KnumCorner] = {
		    {+KWidth / 2.0f, -KHeight / 2.0f, 0},
		    {-KWidth / 2.0f, -KHeight / 2.0f, 0},
		    {+KWidth / 2.0f, +KHeight / 2.0f, 0},
		    {-KWidth / 2.0f, +KHeight / 2.0f, 0},
		};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}
void Player::CheckMapLanding(const CollisionMapInfo& info) {
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {
			std::array<KamataEngine::Vector3, KnumCorner> positionsNew;
			for (uint32_t i = 0; i < positionsNew.size(); i++) {
				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}
			MapChipType mapChipType;
			bool hit = false;
			MapChipField::IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[KleftBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			if (!hit) {
					onGround_ = false;
			}
		}
	} else {
		if (info.landing) {
			onGround_=true;
			velocity_.x *= (1.0f - kAttenuationLanding);
			velocity_.y = 0.0f;
		}
	}
}
