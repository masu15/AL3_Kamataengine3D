#pragma once
#include "KamataEngine.h"
class MapChipField;
enum class LRDirection { 
	kRight,
	KLeft
};
enum Corner 
{
	kRightBottom,//右下
	KleftBottom,//左下
	kRightTop,//右上
	KLeftTop,//左上

	KnumCorner//要素数
};
class Player 
{
public:
	void Initialize(KamataEngine::Model* mod, KamataEngine::Camera * camera,KamataEngine::Vector3 &position);

	void Update();
	
	
	static inline const float KAcceleration = 0.1f;
	static inline const float KAttenuation = 0.1f;
	static inline const float KLimitRunSpeed = 0.5f;
	static inline const float KTimeTurn = 0.3f;
	static inline const float kGravityAcceleration = 0.1f;
	static inline const float kLimitFallSpeed = 1.0f;
	static inline const float KJumpAcceleration = 1.0f;
	static inline const float KWidth = 0.8f;
	static inline const float KHeight = 0.8f;
	static inline const float kBlank = 0.1f;
	static inline const float kAttenuationLanding = 0.0f;
	static inline const float kGroundSearchHeight = 0.1f;
	static inline const float kAttenuationWall = 0.5f;
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const KamataEngine::Vector3&GetVelocity() const { return velocity_; }
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_=mapChipField;}
	struct CollisionMapInfo 
	{
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		KamataEngine::Vector3 move;
	};
	void InputMove();
	void CheckMapCollision(CollisionMapInfo&info);
	void CheckMapMove(const CollisionMapInfo&info);
	void CheckMapCeiling(const CollisionMapInfo& info);
	void CheckMapWall(const CollisionMapInfo& info);
	void CheckMapLanding(const CollisionMapInfo& info);
	void AnimateTurn();
	void Draw();
	void CheckMapCollisionUp(CollisionMapInfo&info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);
	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);
	private:
	MapChipField* mapChipField_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
    KamataEngine::Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::Vector3 velocity_ = {};
	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	bool onGround_ = true;
	
};
