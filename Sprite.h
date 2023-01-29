#pragma once
#include "SpriteCommon.h"

class Sprite
{
private:

	//頂点データ構造体
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	//定数バッファ(マテリアル)
	struct  ConstBufferDataMaterial
	{
		DirectX::XMFLOAT4 color;
	};

	struct ConstBufferDataTransform
	{
		DirectX::XMMATRIX mat;
	};

	//構造体
public:
	enum VertexNumbber
	{
		LB,
		LT,
		RB,
		RT
	};

public:
	//初期化
	void Initialize(SpriteCommon* spriteCommon_);
	//更新
	void Update();
	//描画
	void Draw();

	//ゲッター
	const DirectX::XMFLOAT4& GetColor()const { return color_; }

	const uint32_t GetTextureIndex()const { return textureIndex; }

	const DirectX::XMFLOAT2& GetPosition()const { return position; }
	const float& GetRotationZ()const { return rotationZ; }
	const DirectX::XMFLOAT2& GetSize()const { return size; }
	const DirectX::XMFLOAT2& GetAnchorPoint()const { return anchorPoint; }
	
	const bool& GetIsFlipX() const { return IsFlipX; }
	const bool& GetIsFlipY() const { return IsFlipY; }
	const bool& GetIsInvisible() const { return IsInvisible; }

	//セッター
	void SetColor(DirectX::XMFLOAT4 color) { color_ = color; }

	void SetTextureIndex(uint32_t index) { this->textureIndex = index; }

	void SetPosition(const DirectX::XMFLOAT2& position) { this->position = position; }
	void SetRotation(const float rotationZ) { this->rotationZ = rotationZ; }
	void SetSize(const DirectX::XMFLOAT2& size) { this->size = size; }
	void SetAnchorPoint(const DirectX::XMFLOAT2& anchorPoint) { this->anchorPoint =anchorPoint; }

	void SetIsFlipX(const bool& isFlipX) { this->IsFlipX = isFlipX; }
	void SetIsFlipY(const bool& isFlipY) { this->IsFlipY = isFlipY; }
	void SetIsInvisible(const bool& IsInvisible) { this->IsInvisible = IsInvisible; }


private:
	//スプライト
	SpriteCommon* spriteCommon = nullptr;

	//テクスチャ番号
	uint32_t textureIndex = 0;


	DirectX::XMFLOAT4 color_ = { 1,1,1,1.f };
	
	//スプライト情報
	DirectX::XMFLOAT2 position={100.0f,100.0f};
	float rotationZ = 0.f;
	DirectX::XMFLOAT2 size = { 100.f,100.f };
	//アンカーポイント
	DirectX::XMFLOAT2 anchorPoint = { 0.0f,0.0f };

	bool IsFlipX = false;
	bool IsFlipY = false;

	bool IsInvisible = true;

	//頂点データ
	Vertex vertices[4];
	Vertex* vertMap = nullptr;

	
	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	Microsoft::WRL::ComPtr<ID3D12Resource>constBufferMaterial;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource>constBuffTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;
};

