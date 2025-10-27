#pragma once
#include "Sprite.h"
#include "SpriteManager.h"

// フェードの状態
enum class Status {
	None,
	FadeIn,
	FadeOut,
};

/// <summary>
///  フェード
/// </summary>
class Fade {
public:
	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Draw();
	~Fade();

	// フェード開始
	void Start(Status status, float duration);
	// フェード終了
	void Stop();
	// フェード終了判定
	bool IsFinished() const;

private:
	

	SpriteManager* spriteManager = nullptr;
	std::vector<Sprite*> sprites_;

	Status status_ = Status::None;
	// フェードの持続時間
	float duration_ = 0.0f;
	// 経過時間カウンター
	float counter_ = 0.0f;
};

