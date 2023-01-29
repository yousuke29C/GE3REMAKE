#pragma once
#include <Windows.h>
class WinApp
{
public://静的メンバ関数
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


public:
	//初期化
	void Initialize();
	//更新
	void Update();

	//終了
	void Finalize();

	//メッセージの処理
	bool ProcessMessage();
	
	//getter
	HWND GetHwnd()const { return hwnd; }
	//getter
	HINSTANCE GetHInstance()const { return w.hInstance; }
public://定数
		// ウィンドウサイズ
	static const int window_width = 1280;  // 横幅
	static const int window_height = 720;  // 縦幅

private:
	//ウィンドウクラス
	HWND hwnd = nullptr;
	//ウィンドウクラスの設定
	WNDCLASSEX w{};
};

