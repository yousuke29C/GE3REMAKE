#include "Input.h"
#include "WinApp.h"
#include "DirectXcommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    //ポインタ


#pragma region 基盤システムの初期化
    //windowsAPIの初期化
    WinApp* winApp = nullptr;
    winApp = new WinApp();
    winApp->Initialize();

    // DirectX初期化
    DirectXcommon* dxCommon = nullptr;
    dxCommon = new DirectXcommon();
    dxCommon->Initialize(winApp);


    Input* input = nullptr;
    input = new Input();
    input->Initialize(winApp);

    SpriteCommon* spriteCommon = nullptr;
    spriteCommon = new SpriteCommon;
    spriteCommon->Initialize(dxCommon);
    spriteCommon->LoadTexture(0, "texture.png");
    spriteCommon->LoadTexture(1, "reimu.png");

#pragma endregion 


#pragma region 最初のシーンの初期化
    Sprite* sprite = nullptr;
    sprite = new Sprite;
    sprite->SetTextureIndex(0);
    sprite->Initialize(spriteCommon);
    

#pragma endregion 
    // ゲームループ
    while (true) {
        
        //windowsのメッセージ処理
        if (winApp->ProcessMessage()){
            //ゲームループを抜ける
            break;
        }
#pragma region 最初のシーンの終了

#pragma endregion
        //入力の更新
        input->Update();

        //DirectX::XMFLOAT2 pos = sprite->GetPosition();
        //pos.x += 0.1f;
        //sprite->SetPosition(pos);

        //float pos = sprite->GetRotationZ();
        //pos += 0.1f;
        //sprite->SetRotation(pos);



        sprite->Update();

        //描画前処理
        dxCommon->PreDraw();
        
        spriteCommon->PreDraw();
        sprite->Draw();

#pragma region 最初のシーンを描画

#pragma endregion

        //描画後処理
        dxCommon->PostDraw();

        // DirectX毎フレーム処理　ここまで

    }
#pragma region 基盤システムの更新
    //sprite
    delete sprite;
    sprite = nullptr;
    //spriteCommon
    delete spriteCommon;
    spriteCommon = nullptr;

    delete input;
    input = nullptr;
    
    //WindowsAPIの終了処理
    winApp->Finalize();
    
    delete dxCommon;

    delete winApp;
    winApp = nullptr;
#pragma endregion
    return 0;
}
