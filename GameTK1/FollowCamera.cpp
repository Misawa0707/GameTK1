
#include "FollowCamera.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

// �ÓI�����o�ϐ��̏�����
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int width, int height)
	:Camera(width, height)
{
	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;

	m_Keyboard = nullptr;
	isFPS = false;

	m_player = nullptr;

}

void FollowCamera::Update()
{
	Keyboard::State Keystate = m_Keyboard->GetState();
	m_KeyboardTracker.Update(Keystate);

	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keyboard::C))
	{
		//�J�����̃t���O�؂�ւ�
		isFPS = !isFPS;
	}
	if (m_player)
	{
		//�Ǐ]�Ώۂ̍��W�Ɗp�x���Z�b�g
		SetTargetPos(m_player->GetTrans());
		SetTargetAngle(m_player->GetRot().y);
	}
	// �J�������_���W�A�Q�Ɠ_���W
	Vector3 eyepos, refpos;


	if(isFPS)
	{//FPS�J�����̏���
	 // �Q�Ɠ_���W���v�Z
		eyepos = m_targetPos + Vector3(0, 0.2f, 0);
		// ���@����J�������W�ւ̍���
		Vector3 cameraV(0, 0, -CAMERA_DISTANCE);
		// ���@�̌��ɉ�荞�ވׂ̉�]�s��
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		// �J�����ւ̃x�N�g������]
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		// �J�������W���v�Z
		refpos = eyepos + cameraV;
		
	}
	else
	{//TPS�J�����̏���
		// �Q�Ɠ_���W���v�Z
		refpos = m_targetPos + Vector3(0, 2, 0);
		// ���@����J�������W�ւ̍���
		Vector3 cameraV(0, 0, CAMERA_DISTANCE);
		// ���@�̌��ɉ�荞�ވׂ̉�]�s��
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		// �J�����ւ̃x�N�g������]
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		// �J�������W���v�Z
		eyepos = refpos + cameraV;
	
	//���_�����݈ʒu�����Ԃ���
	eyepos = m_eyepos + (eyepos- m_eyepos)*0.05f;
	//�Q�Ɠ_�����ݒn�����Ԃ���
	refpos = m_refpos + (refpos - m_refpos)*0.2f;
	}

	SetEyePos(eyepos);
	SetRefPos(refpos);

	// ���N���X�̍X�V
	Camera::Update();
}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & targetpos)
{
	m_targetPos = targetpos;
}

void FollowCamera::SetTargetAngle(float targetangle)
{
	m_targetAngle = targetangle;
}

void FollowCamera::SetKeyboard(DirectX::Keyboard * Keyboard)
{
	m_Keyboard = Keyboard;
}
