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

    // 現在のキー状態を取得
    HRESULT result = keyboard_->Acquire();
    if (FAILED(result)) {
        return; // 非アクティブならスキップ
    }

    result = keyboard_->GetDeviceState(sizeof(keys_), keys_);
    if (FAILED(result)) {
        keyboard_->Acquire(); // 再取得を試みる
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
