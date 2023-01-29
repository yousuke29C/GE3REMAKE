#pragma once
#include "WinApp.h"
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800   // DirectInput�̃o�[�W�����w��
#include <dinput.h>
class Input
{
public:
	//namespace�ȗ�
	template<class  T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//������
	void Initialize(WinApp* winApp);
	//�X�V
	void Update();
	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	bool TriggerKey(BYTE keyNumber);

private:
	//windowsAPI
	WinApp* winApp_ = nullptr;

	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> keyboard;
	//DirectInput�̃C���X�^���X
	ComPtr<IDirectInput8> directInput = nullptr;

	BYTE key[256] = {};
	BYTE keyPre[256] = {};

};

