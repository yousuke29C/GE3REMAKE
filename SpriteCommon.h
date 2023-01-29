#pragma once
#include<d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <string>
#include <array>

#include "DirectXcommon.h"

class SpriteCommon
{

public://メンバ関数
	//初期化
	void Initialize(DirectXcommon*dxCommon_);

	//描画
	void PreDraw(); 

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="index">テクスチャ番号</param>
	void LoadTexture(uint32_t index, const std::string& filename);

	/// <summary>
	/// 描画用テクスチャ
	/// </summary>
	/// <param name="">テクスチャ番号</param>
	void SetTextureCommands(uint32_t index);

	//Getter
	DirectXcommon* GetDirectXCommon() { return dxCommon; }



private:	//静的メンバ関数
	//SRVの最大個数
	static const size_t kMaxSRVCount = 2056;

	static std::string kDefaultTextureDirectoryPath;
private:
	DirectXcommon* dxCommon;

	

	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
	//ループシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;

	//テクスチャバッファ
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>,kMaxSRVCount> texBuff;
	//デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
};

