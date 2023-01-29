#pragma once
#include "SpriteCommon.h"

class Sprite
{
private:

	//���_�f�[�^�\����
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	//�萔�o�b�t�@(�}�e���A��)
	struct  ConstBufferDataMaterial
	{
		DirectX::XMFLOAT4 color;
	};

	struct ConstBufferDataTransform
	{
		DirectX::XMMATRIX mat;
	};

	//�\����
public:
	enum VertexNumbber
	{
		LB,
		LT,
		RB,
		RT
	};

public:
	//������
	void Initialize(SpriteCommon* spriteCommon_);
	//�X�V
	void Update();
	//�`��
	void Draw();

	//�Q�b�^�[
	const DirectX::XMFLOAT4& GetColor()const { return color_; }

	const uint32_t GetTextureIndex()const { return textureIndex; }

	const DirectX::XMFLOAT2& GetPosition()const { return position; }
	const float& GetRotationZ()const { return rotationZ; }
	const DirectX::XMFLOAT2& GetSize()const { return size; }
	const DirectX::XMFLOAT2& GetAnchorPoint()const { return anchorPoint; }
	
	const bool& GetIsFlipX() const { return IsFlipX; }
	const bool& GetIsFlipY() const { return IsFlipY; }
	const bool& GetIsInvisible() const { return IsInvisible; }

	//�Z�b�^�[
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
	//�X�v���C�g
	SpriteCommon* spriteCommon = nullptr;

	//�e�N�X�`���ԍ�
	uint32_t textureIndex = 0;


	DirectX::XMFLOAT4 color_ = { 1,1,1,1.f };
	
	//�X�v���C�g���
	DirectX::XMFLOAT2 position={100.0f,100.0f};
	float rotationZ = 0.f;
	DirectX::XMFLOAT2 size = { 100.f,100.f };
	//�A���J�[�|�C���g
	DirectX::XMFLOAT2 anchorPoint = { 0.0f,0.0f };

	bool IsFlipX = false;
	bool IsFlipY = false;

	bool IsInvisible = true;

	//���_�f�[�^
	Vertex vertices[4];
	Vertex* vertMap = nullptr;

	
	//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource>vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	Microsoft::WRL::ComPtr<ID3D12Resource>constBufferMaterial;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource>constBuffTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;
};

