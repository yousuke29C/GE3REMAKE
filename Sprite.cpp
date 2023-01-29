#include "Sprite.h"

using namespace DirectX;
using namespace Microsoft::WRL;

void Sprite::Initialize(SpriteCommon* spriteCommon_)
{
	HRESULT result;
	assert(spriteCommon_);
	spriteCommon = spriteCommon_;

	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;

	if (IsFlipX) {
		left = -left;
		right = -right;
	}
	if (IsFlipY) {
		top = -top;
		bottom = -bottom;
	}

	// ���_�f�[�^
	vertices[LB] = { {   left, bottom, 0.0f}, {0.0f, 1.0f} }; // ����
	vertices[LT] = { {   left,  top, 0.0f}, {0.0f, 0.0f} }; // ����
	vertices[RB] = { { right, bottom, 0.0f}, {1.0f, 1.0f} };// �E��
	vertices[RT] = { { right,   top, 0.0f}, {1.0f, 0.0f} }; // �E��

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// ���_�o�b�t�@�̐���
	result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);

	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_�P���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);

	//�}�e���A��
	{
		// �q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;                   // GPU�ւ̓]���p
		// ���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;   // 256�o�C�g�A���C�������g
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// �萔�o�b�t�@�̐���
		result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
			&cbHeapProp, // �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBufferMaterial));
		assert(SUCCEEDED(result));

		// �萔�o�b�t�@�̃}�b�s���O
		result = constBufferMaterial->Map(0, nullptr, (void**)&constMapMaterial); // �}�b�s���O
		assert(SUCCEEDED(result));

		constMapMaterial->color = color_;
	}

	//�s��
	{
		// �q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;                   // GPU�ւ̓]���p
		// ���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;   // 256�o�C�g�A���C�������g
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// �萔�o�b�t�@�̐���
		result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
			&cbHeapProp, // �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));
		assert(SUCCEEDED(result));

		// �萔�o�b�t�@�̃}�b�s���O
		result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // �}�b�s���O
		assert(SUCCEEDED(result));


		//���[���h
		XMMATRIX matWorld;
		matWorld = XMMatrixIdentity();

		//��]
		XMMATRIX matRot;
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
		matWorld *= matRot;

		//���s
		XMMATRIX matTrans;
		matTrans = XMMatrixTranslation(position.x, position.y,0.0f);
		matWorld *= matTrans;

		//�ˉe�s��
		XMMATRIX matProjection = XMMatrixOrthographicOffCenterLH(
			0.f, WinApp::window_width,
			WinApp::window_height, 0.f,
			0.0f, 1.0f
		);

		constMapTransform->mat = matWorld * matProjection;
	}
}

void Sprite::Update()
{
	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;

	if (IsFlipX) {
		left = -left;
		right = -right;
	}
	if (IsFlipY) {
		top = -top;
		bottom = -bottom;
	}


	// ���_�f�[�^
	vertices[LB] = { { left, bottom, 0.0f}, {0.0f, 1.0f} }; // ����
	vertices[LT] = { { left,  top, 0.0f}, {0.0f, 0.0f} }; // ����
	vertices[RB] = { { right, bottom, 0.0f}, {1.0f, 1.0f} };// �E��
	vertices[RT] = { { right,   top, 0.0f}, {1.0f, 0.0f} }; // �E��
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);

	constMapMaterial->color = color_;

	//���[���h
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();

	//��]
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
	matWorld *= matRot;

	//���s
	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(position.x, position.y, 0.0f);
	matWorld *= matTrans;

	//�ˉe�s��
	XMMATRIX matProjection = XMMatrixOrthographicOffCenterLH(
		0.f, WinApp::window_width,
		WinApp::window_height, 0.f,
		0.0f, 1.0f
	);

	constMapTransform->mat = matWorld * matProjection;
}

void Sprite::Draw()
{
	if (IsInvisible) {
		return ;
	}

	//�e�N�X�`���R�}���h
	spriteCommon->SetTextureCommands(textureIndex);

	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spriteCommon->GetDirectXCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//�萔
	spriteCommon->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBufferMaterial->GetGPUVirtualAddress());
	spriteCommon->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	//�`��R�}���h
	spriteCommon->GetDirectXCommon()->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}
