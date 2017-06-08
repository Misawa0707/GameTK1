//--------------------------------------------------------------------------------------
// �t�@�C����: Enemy.cpp
// �쐬��:
// �쐬��:
// ����:
//--------------------------------------------------------------------------------------

#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Enemy::Enemy(DirectX::Keyboard* keyboard)
: m_cycle(0.0f)
{
	m_pKeyboard = keyboard;

	Initialize();
}

Enemy::~Enemy()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
void Enemy::Initialize()
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

	//�����ʒu
	Vector3 pos;
	pos.x = rand() % 10;
	pos.z = rand() % 10;

	SetTrans(pos);

	m_Timer = 0;

	m_DistAngle = 0;
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void Enemy::Update()
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

	m_Timer--;
	if (m_Timer < 0)
	{
		m_Timer = 60;
		//�ڕW�p�������_���ɒ��I
		//-0.5�`0.5
		float rnd = (float)rand() / RAND_MAX - 0.5f;

		//-90����90
		rnd *= 180.0f;
		rnd = XMConvertToRadians(rnd);
		m_DistAngle += rnd;
	}


	// �����񏈗�
	{
		// ���@�̊p�x����]
		Vector3 rot = GetRot();
		//���̊p�x�ƖڕW�p�x�̍�
		float angle = m_DistAngle - rot.y;
		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}
		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}
		//���
		rot.y += angle * 0.01f;
		SetRot(rot);
	}

	// �@�̂������Ă�����Ɉړ�
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

	Calc();

}

//-----------------------------------------------------------------------------
// �s��X�V
//-----------------------------------------------------------------------------
void Enemy::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end(); it++)
	{
		it->Update();
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void Enemy::Draw()
{
	//�v���C���[�̃��f��
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end(); it++)
	{
		it->Draw();
	}
}

const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
{
	// �^���N�p�[�c�̍��W��Ԃ�
	return m_Obj[0].Gettranslaton();
}

const DirectX::SimpleMath::Vector3 & Enemy::GetRot()
{
	// �^���N�p�[�c�̉�]��Ԃ�
	return m_Obj[0].Getrotation();
}

void Enemy::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[0].Settranslaton(trans);
}

void Enemy::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[0].Setrotation(rot);
}

const DirectX::SimpleMath::Matrix& Enemy::GetLocalWorld()
{
	// �^���N�p�[�c�̃��[���h�s���Ԃ�
	return m_Obj[0].GetWorld();
}