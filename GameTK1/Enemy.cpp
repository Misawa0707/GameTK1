//--------------------------------------------------------------------------------------
// ファイル名: Enemy.cpp
// 作成者:
// 作成日:
// 説明:
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
// 初期化
//-----------------------------------------------------------------------------
void Enemy::Initialize()
{
	//自機パーツの読み込み
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_TANK].LoadModel(L"Resources/tank.cmo");
	m_Obj[PARTS_BODY].LoadModel(L"Resources/body.cmo");
	m_Obj[PARTS_BODY2].LoadModel(L"Resources/body2.cmo");
	m_Obj[PARTS_HAND].LoadModel(L"Resources/hand.cmo");
	m_Obj[PARTS_HEAD].LoadModel(L"Resources/head.cmo");
	m_Obj[PARTS_HAND2].LoadModel(L"Resources/hand.cmo");

	//親子関係の構築(子パーツに親パーツを設定アドレス渡し)
	m_Obj[PARTS_BODY].SetObjParent(&m_Obj[PARTS_TANK]);
	m_Obj[PARTS_BODY2].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_HAND].SetObjParent(&m_Obj[PARTS_BODY2]);
	m_Obj[PARTS_HEAD].SetObjParent(&m_Obj[PARTS_BODY2]);
	m_Obj[PARTS_HAND2].SetObjParent(&m_Obj[PARTS_BODY2]);

	//子パーツの親からのオフセットを設定
	m_Obj[PARTS_BODY].Settranslaton(Vector3(0, 0.3f, 0));
	m_Obj[PARTS_BODY2].Settranslaton(Vector3(0, 0.5f, 0));
	m_Obj[PARTS_HAND].Settranslaton(Vector3(-0.35f, 0.3f, 0.0f));
	m_Obj[PARTS_HEAD].Settranslaton(Vector3(0, 0.4f, 0));
	m_Obj[PARTS_HAND2].Settranslaton(Vector3(0.35f, 0.3f, 0.0f));

	//パーツの初期状態
	m_Obj[PARTS_HAND].Setscale(Vector3(1.5f, 1.5f, 1.5f));
	m_Obj[PARTS_HAND2].Setscale(Vector3(1.5f, 1.5f, 1.5f));
	m_Obj[PARTS_HAND2].Setrotation(Vector3(0, 0, XMConvertToRadians(-30)));
	m_Obj[PARTS_HAND].Setrotation(Vector3(0, 0, XMConvertToRadians(30)));

	//スケール
	m_cycle = 0.0f;

	//初期位置
	Vector3 pos;
	pos.x = rand() % 10;
	pos.z = rand() % 10;

	SetTrans(pos);

	m_Timer = 0;

	m_DistAngle = 0;
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void Enemy::Update()
{
	//状態取得
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	//ギミック
	//回す
	float angle = m_Obj[PARTS_HAND].Getrotation().z;
	m_Obj[PARTS_HAND].Setrotation(Vector3(0, 0, angle - 0.03f));

	//回す
	float angle2 = m_Obj[PARTS_HAND2].Getrotation().z;
	m_Obj[PARTS_HAND2].Setrotation(Vector3(0, 0, angle2 + 0.03f));

	//移動
	//Vector3 pos = m_Obj[PLAYER_PARTS_HAND].Gettranslaton();
	//m_Obj[PLAYER_PARTS_HAND].Settranslaton(pos + Vector3(-0.05f, 0, 0));

	//スケール
	m_cycle += 0.1f;
	float scale = 1.0f + sinf(m_cycle);
	m_Obj[PARTS_HEAD].Setscale(Vector3(scale, scale, scale));

	m_Timer--;
	if (m_Timer < 0)
	{
		m_Timer = 60;
		//目標角をランダムに抽選
		//-0.5～0.5
		float rnd = (float)rand() / RAND_MAX - 0.5f;

		//-90から90
		rnd *= 180.0f;
		rnd = XMConvertToRadians(rnd);
		m_DistAngle += rnd;
	}


	// 左旋回処理
	{
		// 自機の角度を回転
		Vector3 rot = GetRot();
		//今の角度と目標角度の差
		float angle = m_DistAngle - rot.y;
		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}
		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}
		//補間
		rot.y += angle * 0.01f;
		SetRot(rot);
	}

	// 機体が向いてる方向に移動
	{
		// 移動量のベクトル
		Vector3 moveV(0, 0, -0.1f);
		// 移動ベクトルを自機の角度分回転させる
		//moveV = Vector3::TransformNormal(moveV, head_world);
		float angle = m_Obj[0].Getrotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// 自機の座標を移動
		Vector3 pos = m_Obj[0].Gettranslaton();
		m_Obj[0].Settranslaton(pos + moveV);
	}

	Calc();

}

//-----------------------------------------------------------------------------
// 行列更新
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
// 描画
//-----------------------------------------------------------------------------
void Enemy::Draw()
{
	//プレイヤーのモデル
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end(); it++)
	{
		it->Draw();
	}
}

const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
{
	// タンクパーツの座標を返す
	return m_Obj[0].Gettranslaton();
}

const DirectX::SimpleMath::Vector3 & Enemy::GetRot()
{
	// タンクパーツの回転を返す
	return m_Obj[0].Getrotation();
}

void Enemy::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// タンクパーツの座標を設定
	m_Obj[0].Settranslaton(trans);
}

void Enemy::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// タンクパーツの座標を設定
	m_Obj[0].Setrotation(rot);
}

const DirectX::SimpleMath::Matrix& Enemy::GetLocalWorld()
{
	// タンクパーツのワールド行列を返す
	return m_Obj[0].GetWorld();
}