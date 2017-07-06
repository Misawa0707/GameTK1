//--------------------------------------------------------------------------------------
// ファイル名: Player.cpp
// 作成者:
// 作成日:
// 説明:
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
// 初期化
//-----------------------------------------------------------------------------
void Player::Initialize()
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
	//デフォルトで発射していない
	FireFlag = false;


	{//弾丸用のあたり判定ノードの設定
		m_CollisionNodeBullet.Initialize();
		//親パーツを設定
		m_CollisionNodeBullet.SetParent(&m_Obj[PARTS_HAND]);
		m_CollisionNodeBullet.SetTrans(Vector3(0, 0, 0));
		m_CollisionNodeBullet.SetLocalRadius(0.5f);
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void Player::Update()
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

	// 左旋回処理
	if (keystate.A)
	{
		// 自機の角度を回転
		//head_angle += 0.03f;
		float angle = m_Obj[0].Getrotation().y;
		m_Obj[0].Setrotation(Vector3(0, angle + 0.03f, 0));
	}

	// 右旋回処理
	if (keystate.D)
	{
		// 自機の角度を回転
		float angle = m_Obj[0].Getrotation().y;
		m_Obj[0].Setrotation(Vector3(0, angle - 0.03f, 0));
	}

	// 前進処理
	if (keystate.W)
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

	// 後退処理
	if (keystate.S)
	{
		//移動量のベクトル
		Vector3 moveV(0, 0, +0.1f);
		//移動ベクトルを自機の角度分回転させる
		float angle = m_Obj[0].Getrotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		//自機の座標を移動
		Vector3 pos = m_Obj[0].Gettranslaton();
		m_Obj[0].Settranslaton(pos + moveV);
	}
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		// 発射中なら
		if (FireFlag)
		{
			// 戻す
			ResetBullet();
		}
		else
		{
			// 発射
			FireBullet();
		}
	}

	if (FireFlag)
	{
		// 弾丸パーツの座標を移動
		Vector3 pos = m_Obj[PARTS_HAND].Gettranslaton();
		m_Obj[PARTS_HAND].Settranslaton(pos + m_BulletVel);
	}
	m_CollisionNodeBody.Initialize();
	m_CollisionNodeBody.SetParent(&m_Obj[PARTS_BODY2]);


	Calc();

}

//-----------------------------------------------------------------------------
// 行列更新
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
// 描画
//-----------------------------------------------------------------------------
void Player::Draw()
{
	//プレイヤーのモデル
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

	//発射するパーツのワールド行列を取得
	Matrix worldm = m_Obj[PARTS_HAND].GetWorld();

	//抽出した情報をしまっておく
	Vector3 scale;			//ワールドスケーリング
	Quaternion rotation;	//ワールド回転
	Vector3 translation;	//ワールド座標
	//ワールド行列から各要素を抽出
	worldm.Decompose(scale, rotation, translation);
	//親パーツから分離
	m_Obj[PARTS_HAND].SetObjParent(nullptr);
	m_Obj[PARTS_HAND].Setscale(scale);
	m_Obj[PARTS_HAND].SetrotationQ(rotation);
	m_Obj[PARTS_HAND].Settranslaton(translation);

	//弾丸パーツの速度設定
	m_BulletVel = Vector3(0, 0, -0.1f);
	//パーツの向きに合わせて速度ベクトルを回転
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
	// タンクパーツの座標を返す
	return m_Obj[0].Gettranslaton();
}

const DirectX::SimpleMath::Vector3 & Player::GetRot()
{
	// タンクパーツの回転を返す
	return m_Obj[0].Getrotation();
}

void Player::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// タンクパーツの座標を設定
	m_Obj[0].Settranslaton(trans);
}

void Player::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// タンクパーツの座標を設定
	m_Obj[0].Setrotation(rot);
}

const DirectX::SimpleMath::Matrix& Player::GetLocalWorld()
{
	// タンクパーツのワールド行列を返す
	return m_Obj[0].GetWorld();
}