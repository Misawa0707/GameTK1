#include "CollisionNode.h"

using namespace DirectX::SimpleMath;

/// <summary>
/// CollisionNodeのメンバ関数
/// </summary>

void CollisionNode::SetParent(Obj3d * parent)
{
	m_Obj.SetObjParent(parent);
}

/// <summary>
/// SphereNodeのメンバ関数
/// </summary>

SphereNode::SphereNode()
{
	//デフォルトの半径1メートル
	m_LocalRadius = 1.0f;
}

void SphereNode::Initialize()
{
	//デバック表示用モデルの読み込み
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


