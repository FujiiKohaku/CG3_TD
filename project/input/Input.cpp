#include "Input.h"
#include <cassert>

bool Input::Initialize(WinApp* winApp)
{
    HRESULT result;

    winApp_ = winApp;

    // DirectInput全体の初期化
    result = DirectInput8Create(
        winApp_->GetHinstance(),
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        reinterpret_cast<void**>(directInput_.ReleaseAndGetAddressOf()),
        nullptr);
    assert(SUCCEEDED(result));

    // キーボードデバイスの生成
    result = directInput_->CreateDevice(GUID_SysKeyboard, keyboard_.ReleaseAndGetAddressOf(), nullptr);
    assert(SUCCEEDED(result));

    // 入力データ形式のセット
    result = keyboard_->SetDataFormat(&c_dfDIKeyboard);
    assert(SUCCEEDED(result));

    // 排他制御レベルのセット
    result = keyboard_->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));

    return true;
}

void Input::Update()
{

    // 前フレームのキー状態をコピーして保持
    memcpy(preKeys_, keys_, sizeof(keys_));

    prePushB_ = isPushB_;
    prePushLeft_ = isPushLeft_;
    prePushRight_ = isPushRight_;
    //ここで戻す
    isPushB_ = false;
    isPushLeft_ = false;
    isPushRight_ = false;
    // 現在のキー状態を取得
    HRESULT result = keyboard_->Acquire();
    if (FAILED(result)) {
        return; // 非アクティブならスキップ
    }

    result = keyboard_->GetDeviceState(sizeof(keys_), keys_);
    if (FAILED(result)) {
        keyboard_->Acquire(); // 再取得を試みる
    }

    //=========================
    // xboxコントローラーの入力処理
    //=========================
    // xboxコントローラーの入力情報を初期化
    ZeroMemory(&xinputState_, sizeof(xinputState_));

    // xboxコントローラーの情報の取得開始
    DWORD conResult = XInputGetState(0, &xinputState_); // 0~3までの4つのコントローラーに対応可

    // xboxコントローラー接続時
    if (conResult == ERROR_SUCCESS) {

        // ボタン
        if (xinputState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) { /* Aボタンを押したときの処理 */
        }
        if (xinputState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
            isPushB_ = true;
        }
        if (xinputState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) { /* Xボタンを押したときの処理 */
        }
        if (xinputState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) { /* Yボタンを押したときの処理 */
        }

        // 十字キー
        if (xinputState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) { /* ↑を押したときの処理 */
        }
        if (xinputState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) { /* ↓を押したときの処理 */
        }
        if (xinputState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
            isPushLeft_ = true;
        }
        if (xinputState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
            isPushRight_ = true;
        }
    }
}

bool Input::IsKeyPressed(BYTE keyNumber) const
{
    if (keys_[keyNumber]) {
        return true;
    }
    return false;
}

bool Input::IsKeyTriggered(BYTE keyNumber) const
{
    if (keys_[keyNumber] && !preKeys_[keyNumber]) {
        return true;
    }
    return false;
}

bool Input::IsTriggerB()
{
    if (isPushB_ && !prePushB_) {
        return true;
    }
    return false;
}

bool Input::IsTriggerLeftButton()
{
    if (isPushLeft_ && !prePushLeft_) {
        return true;
    }
    return false;
}

bool Input::IsTriggerRightButton()
{
    if (isPushRight_ && !prePushRight_) {
        return true;
    }
    return false;
}
