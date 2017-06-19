#include "CollisionNode.h"

using namespace DirectX::SimpleMath;

/// <summary>
/// CollisionNode�̃����o�֐�
/// </summary>

void CollisionNode::SetParent(Obj3d * parent)
{
	m_Obj.SetObjParent(parent);
}

/// <summary>
/// SphereNode�̃����o�֐�
/// </summary>

SphereNode::SphereNode()
{
	//�f�t�H���g�̔��a1���[�g��
	m_LocalRadius = 1.0f;
}

void SphereNode::Initialize()
{
	//�f�o�b�N�\���p���f���̓ǂݍ���
	m_Obj.LoadModel(L"Resources/SphereNode.cmo");
}

void SphereNode::Update()
{
	m_Obj.Settranslaton(m_Trans);
	m_Obj.Setscale(Vector3(m_LocalRadius));
	m_Obj.Update();
}

void SphereNode::Draw()
{
	m_Obj.Draw();
}


