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
	//�X�P�[�����O�iXYZ)
	//setter
	void Setscale(const DirectX::SimpleMath::Vector3& scale)		{ m_scale = scale; }
	//��]�p�iXYZ)
	void Setrotation(const DirectX::SimpleMath::Vector3& rotation) { m_rotation = rotation; }
	//���s�ړ��iXYZ)
	void Settranslaton(const DirectX::SimpleMath::Vector3& translaton) { m_translaton = translaton; }
	//�e�̂RD�I�u�W�F�N�g�ւ̃|�C���^
	void SetObjParent(Obj3d* ObjParent) { m_ObjParent = ObjParent; }
	//�X�P�[�����O�iXYZ)
	//getter
	const DirectX::SimpleMath::Vector3& Getscale() { return m_scale; }
	//��]�p�iXYZ)
	const DirectX::SimpleMath::Vector3& Getrotation() { return m_rotation; }
	//���s�ړ��iXYZ)
	const DirectX::SimpleMath::Vector3& Gettranslaton() { return m_translaton; }
	//���[���h�s��
	const DirectX::SimpleMath::Matrix& GetWorld() { return m_world; }

private:
	//�����o�ϐ�
	//���f��
	std::unique_ptr<DirectX::Model> m_model;
	//�X�P�[�����O�iXYZ)
	DirectX::SimpleMath::Vector3 m_scale;
	//��]�p�iXYZ)
	DirectX::SimpleMath::Vector3 m_rotation;
	//���s�ړ��iXYZ)
	DirectX::SimpleMath::Vector3 m_translaton;
	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//�e��Obj3d�ւ̃|�C���^
	Obj3d* m_ObjParent;

};
