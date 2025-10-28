#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include "WinApp.h"
#include <dinput.h>
#include <wrl.h> // ComPtr 用
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <XInput.h> // xboxコントローラー使うから
#pragma comment(lib,"xinput.lib")

class Input {
public:
    bool Initialize(WinApp* winApp);
    void Update();
    bool IsKeyPressed(BYTE keyNumber) const;
    bool IsKeyTriggered(BYTE keyNumber) const;

    bool IsTriggerB();
    bool IsTriggerLeftButton();
    bool IsTriggerRightButton();

    // getter
    const BYTE* GetKeys() const { return keys_; }
    const BYTE* GetPreKeys() const { return preKeys_; }

private:
    Microsoft::WRL::ComPtr<IDirectInput8> directInput_ = nullptr;
    Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_;
    BYTE keys_[256] {};
    BYTE preKeys_[256] {};
    WinApp* winApp_ = nullptr;

    // xboxコントローラーの初期化
    XINPUT_STATE xinputState_;

    int isPushB_ = false;
    int isPushRight_ = false;
    int isPushLeft_ = false;

    int prePushB_ = false;
    int prePushRight_ = false;
    int prePushLeft_ = false;

};
