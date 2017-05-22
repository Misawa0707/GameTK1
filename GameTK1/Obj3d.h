/// <summary>
/// 3DOBjcet�̃N���X
/// </summary>
#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

#include "Camera.h"

class Obj3d
{
public:
	//�ÓI�����o�֐�
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device>            d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>     d3dContext,
		Camera* camera);

private:
	//�ÓI�����o�ϐ�
	//�f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
	//�f�o�C�X�R���e�L�X�g
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	// �J����
	static Camera* m_Camera;
	// �ėp�X�e�[�g�ݒ�
	static std::unique_ptr<DirectX::CommonStates> m_states;
	// �G�t�F�N�g�t�@�N�g��
	static std::unique_ptr<DirectX::EffectFactory> m_factory;

public:
	//�R���X�g���N�^
	Obj3d();
	//CMO���f���̓ǂݍ���
	void LoadModel(const wchar_t* fileName);
	//�X�V
	void Update();
	//�`��
	void Draw();

	//�����o�֐�

	//�X�P�[�����O�iXYZ)
	//setter

	//��]�p�iXYZ)
	
	//���s�ړ��iXYZ)

	//���[���h�s��

	//�e�̂RD�I�u�W�F�N�g�ւ̃|�C���^

	//�X�P�[�����O�iXYZ)
	//getter

	//��]�p�iXYZ)

	//���s�ړ��iXYZ)

	//���[���h�s��

private:
	//�����o�ϐ�

	//���f��

	//�X�P�[�����O�iXYZ)

	//��]�p�iXYZ)

	//���s�ړ��iXYZ)

	//���[���h�s��
	
	//�e��Obj3d�ւ̃|�C���^
	Obj3d* m_ObjParent;

};
