#pragma once
#include "BaseScene.h"

class Fade;

class StageSelectScene : public BaseScene{
public:
	void Initialize()override;
	void Update(Input* input)override;
	void Draw()override;
	void Finalize()override;

private:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	int selected_ = 0;
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;
};

