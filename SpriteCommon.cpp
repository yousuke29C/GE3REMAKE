#include "SpriteCommon.h"

#include <d3dx12.h>
#include <d3dcompiler.h>
#include <cassert>
#include <DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

std::string SpriteCommon::kDefaultTextureDirectoryPath="Resources/";

void SpriteCommon::Initialize(DirectXcommon* dxCommon_)
{
	HRESULT result{};
	assert(dxCommon_);
	dxCommon = dxCommon_;

    //�f�X�N���v�^�q�[�v�̐ݒ�
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    srvHeapDesc.NumDescriptors = kMaxSRVCount;

    //�ݒ�
    result = dxCommon->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
    assert(SUCCEEDED(result));

    

	//�V�F�[�_�[
    ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
    ComPtr<ID3DBlob> psBlob; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
    ComPtr<ID3DBlob> errorBlob;// �G���[�I�u�W�F�N�g

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        L"Resources/shaders/SpriteVS.hlsl",  // �V�F�[�_�t�@�C����
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
        "main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
        0,
        &vsBlob, &errorBlob
    );
    // �G���[�Ȃ�
    if (FAILED(result)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string error;
        error.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            error.begin());
        error += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(error.c_str());
        assert(0);
    }

    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        L"Resources/shaders/SpritePS.hlsl",   // �V�F�[�_�t�@�C����
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
        "main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
        0,
        &psBlob, &errorBlob
    );
    // �G���[�Ȃ�
    if (FAILED(result)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string error;
        error.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            error.begin());
        error += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(error.c_str());
        assert(0);
    }

    // ���_���C�A�E�g
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        { // xyz���W(1�s�ŏ������ق������₷��)
            "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
        { // uv���W
            "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
    };

    // �O���t�B�b�N�X�p�C�v���C���ݒ�
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
    // �V�F�[�_�[�̐ݒ�
    pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
    pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
    pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
    pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
    // �T���v���}�X�N�̐ݒ�
    pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
    // ���X�^���C�U�̐ݒ�
    pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // �J�����O���Ȃ�
    pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
    pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����
    // �u�����h�X�e�[�g
    pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA�S�Ẵ`�����l����`��
    // ���_���C�A�E�g�̐ݒ�
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
    // �}�`�̌`��ݒ�
    pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    // �����_�[�^�[�Q�b�g
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA�S�Ẵ`�����l����`��

    blenddesc.BlendEnable = true;
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
    blenddesc.SrcBlend = D3D12_BLEND_ONE;
    blenddesc.DestBlend = D3D12_BLEND_ONE;

    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
    blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
    blenddesc.DestBlend = D3D12_BLEND_ZERO;

    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    blenddesc.DestBlend  = D3D12_BLEND_INV_SRC_ALPHA;
    // ���̑��̐ݒ�
    pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
    pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
    pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

    //�f�X�N���v�^�����W
    D3D12_DESCRIPTOR_RANGE descriptorRange{};
    descriptorRange.NumDescriptors = 1;
    descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorRange.BaseShaderRegister = 0;
    descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    //���[�g�p�����[�^�[
    D3D12_ROOT_PARAMETER rootParams[3] = {};
    rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParams[0].Descriptor.ShaderRegister = 0;
    rootParams[0].Descriptor.RegisterSpace = 0;
    rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    //�e�N�X�`�����W�X�^
    rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
    rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
    rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    //�萔�o�b�t�@
    rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParams[2].Descriptor.ShaderRegister = 1;
    rootParams[2].Descriptor.RegisterSpace = 0;
    rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

    // �e�N�X�`���T���v���[�̐ݒ�
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //���J��Ԃ��i�^�C�����O�j
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //�c�J��Ԃ��i�^�C�����O�j
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //���s�J��Ԃ��i�^�C�����O�j
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;  //�{�[�_�[�̎��͍�
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;                   //�S�ă��j�A���
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                                 //�~�b�v�}�b�v�ő�l
    samplerDesc.MinLOD = 0.0f;                                              //�~�b�v�}�b�v�ŏ��l
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    // ���[�g�V�O�l�`��
    // ���[�g�V�O�l�`���̐ݒ�
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = rootParams;
    rootSignatureDesc.NumParameters = _countof(rootParams);
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;

    // ���[�g�V�O�l�`���̃V���A���C�Y
    ID3DBlob* rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    assert(SUCCEEDED(result));
    result = dxCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
    assert(SUCCEEDED(result));
    rootSigBlob->Release();
    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
    pipelineDesc.pRootSignature = rootSignature.Get();
    // �p�C�v�����X�e�[�g�̐���
    result = dxCommon->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
    assert(SUCCEEDED(result));

}

void SpriteCommon::PreDraw()
{
   
}

void SpriteCommon::LoadTexture(uint32_t index,const std::string& filename)
{
    std::string fullPath = kDefaultTextureDirectoryPath + filename;

    int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, nullptr, 0);

    std::vector<wchar_t>wfilePath(filePathBufferSize);
    MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilePath.data(), filePathBufferSize);

    //�t�@�C������
    TexMetadata metadata{};
    ScratchImage scratchImg{};
    //WIC�e�N�X�`���̃��[�h
    HRESULT result = LoadFromWICFile(
        wfilePath.data(),
        WIC_FLAGS_NONE,
        &metadata, scratchImg
    );
    assert(SUCCEEDED(result));

    ScratchImage mipChain{};
    //�~�b�v�}�b�v����
    result = GenerateMipMaps(
        scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
        TEX_FILTER_DEFAULT, 0, mipChain);
    if (SUCCEEDED(result)) {
        scratchImg = std::move(mipChain);
        metadata = scratchImg.GetMetadata();
    }
    // �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
    metadata.format = MakeSRGB(metadata.format);

    //����

    //�e�N�X�`��
    D3D12_HEAP_PROPERTIES cbHeapProp{};
    cbHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    cbHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    cbHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

    D3D12_RESOURCE_DESC textureResourceDesc{};
    textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureResourceDesc.Format = metadata.format;
    textureResourceDesc.Width = metadata.width;
    textureResourceDesc.Height = (UINT)metadata.height;
    textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
    textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
    textureResourceDesc.SampleDesc.Count = 1;

    // �萔�o�b�t�@�̐���
    result = dxCommon->GetDevice()->CreateCommittedResource(
        &cbHeapProp, // �q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &textureResourceDesc, // ���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&texBuff[index])
    );

    //�]��
    for (size_t i = 0; i < metadata.mipLevels; i++)
    {
        //�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
        const Image* img = scratchImg.GetImage(i, 0, 0);
        //�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
        result = texBuff[index]->WriteToSubresource(
            (UINT)i,
            nullptr,//�S�̈�փR�s�[
            img->pixels,//���f�[�^�A�h���X
            (UINT)img->rowPitch,//1���C���T�C�Y
            (UINT)img->slicePitch//�S�T�C�Y
        );
        assert(SUCCEEDED(result));
    }

    //SRV�쐬
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureResourceDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

    //�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
    UINT descriptorhandleInsrementSize = dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    dxCommon->GetDevice()->CreateShaderResourceView
    (
        texBuff[index].Get(),
        &srvDesc,
        CD3DX12_CPU_DESCRIPTOR_HANDLE(
        srvHeap->GetCPUDescriptorHandleForHeapStart(),
            index,
            descriptorhandleInsrementSize)
    );
}

void SpriteCommon::SetTextureCommands(uint32_t index)
{
    //�v���~�e�B�u�`��̐ݒ�R�}���h
    dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�
    dxCommon->GetCommandList()->SetPipelineState(pipelineState.Get());
    dxCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

    //�f�X�N���v�^
    ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
    dxCommon->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    //SRV�q�[�v
    UINT descriptorhandleInsrementSize = dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, 
        CD3DX12_GPU_DESCRIPTOR_HANDLE(
        srvHeap->GetGPUDescriptorHandleForHeapStart(),
        index,
        descriptorhandleInsrementSize));

}
