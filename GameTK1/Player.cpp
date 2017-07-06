//--------------------------------------------------------------------------------------
// �t�@�C����: Player.cpp
// �쐬��:
// �쐬��:
// ����:
//--------------------------------------------------------------------------------------

#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player(DirectX::Keyboard* keyboard)
: m_cycle(0.0f)
{
	m_pKeyboard = keyboard;

	Initialize();
}

Player::~Player()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
void Player::Initialize()
{
	//���@�p�[�c�̓ǂݍ���
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_TANK].LoadModel(L"Resources/tank.cmo");
	m_Obj[PARTS_BODY].LoadModel(L"Resources/body.cmo");
	m_Obj[PARTS_BODY2].LoadModel(L"Resources/body2.cmo");
	m_Obj[PARTS_HAND].LoadModel(L"Resources/hand.cmo");
	m_Obj[PARTS_HEAD].LoadModel(L"Resources/head.cmo");
	m_Obj[PARTS_HAND2].LoadModel(L"Resources/hand.cmo");

	//�e�q�֌W�̍\�z(�q�p�[�c�ɐe�p�[�c��ݒ�A�h���X�n��)
	m_Obj[PARTS_BODY].SetObjParent(&m_Obj[PARTS_TANK]);
	m_Obj[PARTS_BODY2].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_HAND].SetObjParent(&m_Obj[PARTS_BODY2]);
	m_Obj[PARTS_HEAD].SetObjParent(&m_Obj[PARTS_BODY2]);
	m_Obj[PARTS_HAND2].SetObjParent(&m_Obj[PARTS_BODY2]);

	//�q�p�[�c�̐e����̃I�t�Z�b�g��ݒ�
	m_Obj[PARTS_BODY].Settranslaton(Vector3(0, 0.3f, 0));
	m_Obj[PARTS_BODY2].Settranslaton(Vector3(0, 0.5f, 0));
	m_Obj[PARTS_HAND].Settranslaton(Vector3(-0.35f, 0.3f, 0.0f));
	m_Obj[PARTS_HEAD].Settranslaton(Vector3(0, 0.4f, 0));
	m_Obj[PARTS_HAND2].Settranslaton(Vector3(0.35f, 0.3f, 0.0f));

	//�p�[�c�̏������
	m_Obj[PARTS_HAND].Setscale(Vector3(1.5f, 1.5f, 1.5f));
	m_Obj[PARTS_HAND2].Setscale(Vector3(1.5f, 1.5f, 1.5f));
	m_Obj[PARTS_HAND2].Setrotation(Vector3(0, 0, XMConvertToRadians(-30)));
	m_Obj[PARTS_HAND].Setrotation(Vector3(0, 0, XMConvertToRadians(30)));

	//�X�P�[��
	m_cycle = 0.0f;
	//�f�t�H���g�Ŕ��˂��Ă��Ȃ�
	FireFlag = false;


	{//�e�ۗp�̂����蔻��m�[�h�̐ݒ�
		m_CollisionNodeBullet.Initialize();
		//�e�p�[�c��ݒ�
		m_CollisionNodeBullet.SetParent(&m_Obj[PARTS_HAND]);
		m_CollisionNodeBullet.SetTrans(Vector3(0, 0, 0));
		m_CollisionNodeBullet.SetLocalRadius(0.5f);
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void Player::Update()
{
	//��Ԏ擾
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	//�M�~�b�N
	//��
	float angle = m_Obj[PARTS_HAND].Getrotation().z;
	m_Obj[PARTS_HAND].Setrotation(Vector3(0, 0, angle - 0.03f));

	//��
	float angle2 = m_Obj[PARTS_HAND2].Getrotation().z;
	m_Obj[PARTS_HAND2].Setrotation(Vector3(0, 0, angle2 + 0.03f));

	//�ړ�
	//Vector3 pos = m_Obj[PLAYER_PARTS_HAND].Gettranslaton();
	//m_Obj[PLAYER_PARTS_HAND].Settranslaton(pos + Vector3(-0.05f, 0, 0));

	//�X�P�[��
	m_cycle += 0.1f;
	float scale = 1.0f + sinf(m_cycle);
	m_Obj[PARTS_HEAD].Setscale(Vector3(scale, scale, scale));

	// �����񏈗�
	if (keystate.A)
	{
		// ���@�̊p�x����]
		//head_angle += 0.03f;
		float angle = m_Obj[0].Getrotation().y;
		m_Obj[0].Setrotation(Vector3(0, angle + 0.03f, 0));
	}

	// �E���񏈗�
	if (keystate.D)
	{
		// ���@�̊p�x����]
		float angle = m_Obj[0].Getrotation().y;
		m_Obj[0].Setrotation(Vector3(0, angle - 0.03f, 0));
	}

	// �O�i����
	if (keystate.W)
	{
		// �ړ��ʂ̃x�N�g��
		Vector3 moveV(0, 0, -0.1f);
		// �ړ��x�N�g�������@�̊p�x����]������
		//moveV = Vector3::TransformNormal(moveV, head_world);
		float angle = m_Obj[0].Getrotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// ���@�̍��W���ړ�
		Vector3 pos = m_Obj[0].Gettranslaton();
		m_Obj[0].Settranslaton(pos + moveV);
	}

	// ��ޏ���
	if (keystate.S)
	{
		//�ړ��ʂ̃x�N�g��
		Vector3 moveV(0, 0, +0.1f);
		//�ړ��x�N�g�������@�̊p�x����]������
		float angle = m_Obj[0].Getrotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		//���@�̍��W���ړ�
		Vector3 pos = m_Obj[0].Gettranslaton();
		m_Obj[0].Settranslaton(pos + moveV);
	}
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		// ���˒��Ȃ�
		if (FireFlag)
		{
			// �߂�
			ResetBullet();
		}
		else
		{
			// ����
			FireBullet();
		}
	}

	if (FireFlag)
	{
		// �e�ۃp�[�c�̍��W���ړ�
		Vector3 pos = m_Obj[PARTS_HAND].Gettranslaton();
		m_Obj[PARTS_HAND].Settranslaton(pos + m_BulletVel);
	}
	m_CollisionNodeBody.Initialize();
	m_CollisionNodeBody.SetParent(&m_Obj[PARTS_BODY2]);


	Calc();

}

//-----------------------------------------------------------------------------
// �s��X�V
//-----------------------------------------------------------------------------
void Player::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end(); it++)
	{
		it->Update();
	}
	m_CollisionNodeBullet.Update();
	m_CollisionNodeBody.Update();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void Player::Draw()
{
	//�v���C���[�̃��f��
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end(); it++)
	{
		it->Draw();
	}
	m_CollisionNodeBullet.Draw();
	m_CollisionNodeBody.Draw();
}

void Player::FireBullet()
{
	
	if (FireFlag)  return;

	//���˂���p�[�c�̃��[���h�s����擾
	Matrix worldm = m_Obj[PARTS_HAND].GetWorld();

	//���o�����������܂��Ă���
	Vector3 scale;			//���[���h�X�P�[�����O
	Quaternion rotation;	//���[���h��]
	Vector3 translation;	//���[���h���W
	//���[���h�s�񂩂�e�v�f�𒊏o
	worldm.Decompose(scale, rotation, translation);
	//�e�p�[�c���番��
	m_Obj[PARTS_HAND].SetObjParent(nullptr);
	m_Obj[PARTS_HAND].Setscale(scale);
	m_Obj[PARTS_HAND].SetrotationQ(rotation);
	m_Obj[PARTS_HAND].Settranslaton(translation);

	//�e�ۃp�[�c�̑��x�ݒ�
	m_BulletVel = Vector3(0, 0, -0.1f);
	//�p�[�c�̌����ɍ��킹�đ��x�x�N�g������]
	m_BulletVel = Vector3::Transform(m_BulletVel, rotation);

	FireFlag = true;

}

void Player::ResetBullet()
{
	if (!FireFlag)	return;
	m_Obj[PARTS_HAND].SetObjParent(&m_Obj[PARTS_BODY2]);
	m_Obj[PARTS_HAND].Settranslaton(Vector3(-0.35f, 0.3f, 0.0f));
	m_Obj[PARTS_HAND].Setscale(Vector3(1.5f, 1.5f, 1.5f));
	m_Obj[PARTS_HAND].Setrotation(Vector3(0, 0, XMConvertToRadians(30)));

	FireFlag = false;
}

const DirectX::SimpleMath::Vector3& Player::GetTrans()
{
	// �^���N�p�[�c�̍��W��Ԃ�
	return m_Obj[0].Gettranslaton();
}

const DirectX::SimpleMath::Vector3 & Player::GetRot()
{
	// �^���N�p�[�c�̉�]��Ԃ�
	return m_Obj[0].Getrotation();
}

void Player::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[0].Settranslaton(trans);
}

void Player::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[0].Setrotation(rot);
}

const DirectX::SimpleMath::Matrix& Player::GetLocalWorld()
{
	// �^���N�p�[�c�̃��[���h�s���Ԃ�
	return m_Obj[0].GetWorld();
}