#pragma once
#include "Sprite.h"
#include "SpriteManager.h"

// �t�F�[�h�̏��
enum class Status {
	None,
	FadeIn,
	FadeOut,
};

/// <summary>
///  �t�F�[�h
/// </summary>
class Fade {
public:
	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Draw();
	~Fade();

	// �t�F�[�h�J�n
	void Start(Status status, float duration);
	// �t�F�[�h�I��
	void Stop();
	// �t�F�[�h�I������
	bool IsFinished() const;

private:
	

	SpriteManager* spriteManager = nullptr;
	std::vector<Sprite*> sprites_;

	Status status_ = Status::None;
	// �t�F�[�h�̎�������
	float duration_ = 0.0f;
	// �o�ߎ��ԃJ�E���^�[
	float counter_ = 0.0f;
};

