#pragma once

#include "Stage/SceneParam.h"

class SceneManager;
class Input;
class WinApp;
class DirectXCommon;

class BaseScene {
public:
    virtual ~BaseScene() = default;
    virtual void Initialize() = 0;
    virtual void Update(Input* input) = 0;
    virtual void Draw() = 0;
    virtual void Finalize() = 0;

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
    virtual void SetSystem(WinApp* wa, DirectXCommon* dx) { winApp_ = wa; dxCommon_ = dx; }
    virtual void SetParam(const SceneParam& p) { param_ = p; }


protected:
	SceneManager* GetSceneManager() const { return sceneManager_; }
    WinApp* GetWinApp() const { return winApp_; }
    DirectXCommon* GetDx() const { return dxCommon_; }
    const SceneParam& GetParam()const { return param_; }


private:
	SceneManager* sceneManager_ = nullptr;
    WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
    SceneParam param_;
};

