#pragma once
#include "WinApp.h"
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>
class Input
{
public:
	//namespace省略
	template<class  T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//初期化
	void Initialize(WinApp* winApp);
	//更新
	void Update();
	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	bool TriggerKey(BYTE keyNumber);

private:
	//windowsAPI
	WinApp* winApp_ = nullptr;

	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard;
	//DirectInputのインスタンス
	ComPtr<IDirectInput8> directInput = nullptr;

	BYTE key[256] = {};
	BYTE keyPre[256] = {};

};

