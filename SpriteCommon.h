#pragma once
#include<d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <string>
#include <array>

#include "DirectXcommon.h"

class SpriteCommon
{

public://�����o�֐�
	//������
	void Initialize(DirectXcommon*dxCommon_);

	//�`��
	void PreDraw(); 

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="index">�e�N�X�`���ԍ�</param>
	void LoadTexture(uint32_t index, const std::string& filename);

	/// <summary>
	/// �`��p�e�N�X�`��
	/// </summary>
	/// <param name="">�e�N�X�`���ԍ�</param>
	void SetTextureCommands(uint32_t index);

	//Getter
	DirectXcommon* GetDirectXCommon() { return dxCommon; }



private:	//�ÓI�����o�֐�
	//SRV�̍ő��
	static const size_t kMaxSRVCount = 2056;

	static std::string kDefaultTextureDirectoryPath;
private:
	DirectXcommon* dxCommon;

	

	//�p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
	//���[�v�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;

	//�e�N�X�`���o�b�t�@
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>,kMaxSRVCount> texBuff;
	//�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
};

