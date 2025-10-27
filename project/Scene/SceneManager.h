#pragma once
#include "BaseScene.h"
#include "SceneParam.h"

class WinApp;
class DirectXCommon;

class SceneManager {
public:
	void Update(Input* input);
	void Draw();
	~SceneManager();

	void SetNextScene(BaseScene* nextScene);
	void SetNextScene(BaseScene* nextScene, const SceneParam& param);
	void SetSystem(WinApp* wa, DirectXCommon* dx) { winApp_ = wa; dxCommon_ = dx; }

private:
	// 今のシーン
	BaseScene* scene_ = nullptr;
	// 次のシーン
	BaseScene* nextScene_ = nullptr;

	// 借り物
	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
};

