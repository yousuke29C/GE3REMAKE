#pragma once
#include <Windows.h>
class WinApp
{
public://�ÓI�����o�֐�
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


public:
	//������
	void Initialize();
	//�X�V
	void Update();

	//�I��
	void Finalize();

	//���b�Z�[�W�̏���
	bool ProcessMessage();
	
	//getter
	HWND GetHwnd()const { return hwnd; }
	//getter
	HINSTANCE GetHInstance()const { return w.hInstance; }
public://�萔
		// �E�B���h�E�T�C�Y
	static const int window_width = 1280;  // ����
	static const int window_height = 720;  // �c��

private:
	//�E�B���h�E�N���X
	HWND hwnd = nullptr;
	//�E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
};

