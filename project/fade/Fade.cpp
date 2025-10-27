#include "Fade.h"
#include "TextureManager.h"
#include <iostream>
#include <algorithm>

void Fade::Initialize(DirectXCommon* dxCommon) {
	assert(dxCommon);
	TextureManager::GetInstance()->Initialize(dxCommon);
	TextureManager::GetInstance()->LoadTexture("resources/black2x2.png");
	spriteManager = new SpriteManager();
	spriteManager->Initialize(dxCommon); // DX12の共通情報はnullptrで初期化（実際の描画はしないので問題なし）
	
	for (int i = 0; i < 4; i++) {
	    Sprite* sprite = new Sprite();
	    sprite->Initialize(spriteManager, "resources/black2x2.png");
		sprite->SetPosition({ 640.0f * float(i%2) * float(i-2), 0.0f});
		if (i == 0) {// 上
			sprite->SetPosition({ 0.0f, -360.0f });
			sprite->SetSize({ 1280.0f, 360.0f });
		}
		if (i == 1) {// 左
			sprite->SetPosition({ -640.0f,0.0f });
			sprite->SetSize({ 640.0f, 720.0f });
		}
		if (i == 2) {// 下
			sprite->SetPosition({ 0.0f,720.0f });
			sprite->SetSize({ 1280.0f, 360.0f });
		}
		if (i == 3) {// 右
			sprite->SetPosition({ 1280.0f,0.0f });
			sprite->SetSize({ 640.0f, 720.0f });
		}
		sprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	    sprites_.push_back(sprite);
	}
}

void Fade::Update() {
	for(auto sprite : sprites_) {
		sprite->Update();
	}
	// フェード状態による分岐
	switch (status_) {
	case Status::None:
		break;
	case Status::FadeIn:
		// 1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		// フェード継続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}

		for(int i = 0; i < 4; i++) {
			if (i == 0) {// 上
				sprites_[i]->SetPosition({ 0.0f, -360.0f + std::clamp(360.0f * ((duration_ - counter_) / duration_),0.0f,360.0f) });
			}
			if (i == 1) {// 左
				sprites_[i]->SetPosition({ -640.0f + std::clamp(640.0f * ((duration_ - counter_) / duration_),0.0f,640.0f),0.0f });
			}
			if (i == 2) {// 下
				sprites_[i]->SetPosition({ 0.0f,720.0f - std::clamp(360.0f * ((duration_ - counter_) / duration_),0.0f,360.0f) });
			}
			if (i == 3) {// 右
				sprites_[i]->SetPosition({ 1280.0f - std::clamp(640.0f * ((duration_ - counter_) / duration_),0.0f,640.0f),0.0f });
			}
		}

		break;
	case Status::FadeOut:
		// 1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		// フェード継続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		for (int i = 0; i < 4; i++) {
			if (i == 0) {// 上
				sprites_[i]->SetPosition({ 0.0f, -360.0f + std::clamp(360.0f * (counter_ / duration_),0.0f,360.0f) });
			}
			if (i == 1) {// 左
				sprites_[i]->SetPosition({ -640.0f + std::clamp(640.0f * (counter_ / duration_),0.0f,640.0f),0.0f });
			}
			if (i == 2) {// 下
				sprites_[i]->SetPosition({ 0.0f,720.0f - std::clamp(360.0f * (counter_ / duration_),0.0f,360.0f) });
			}
			if (i == 3) {// 右
				sprites_[i]->SetPosition({ 1280.0f - std::clamp(640.0f * (counter_ / duration_),0.0f,640.0f),0.0f });
			}
		}
		break;
	}
}

void Fade::Draw() {
	if(status_ == Status::None) {
		return;
	}
	spriteManager->PreDraw();
	for(auto sprite : sprites_) {
		sprite->Draw();
	}
}

Fade::~Fade() {
	delete spriteManager;
	for (auto sprite : sprites_) {
		delete sprite;
	}
}

void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

void Fade::Stop() {
	status_ = Status::None;
	counter_ = 0.0f;
}

bool Fade::IsFinished() const {
	// フェード状態による分岐
	switch (status_) {
	case Status::FadeIn:
	case Status::FadeOut:
		if(counter_>= duration_) {
			return true;
		} else {
			return false;
		}
	}
	return true;
}
