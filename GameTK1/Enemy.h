//--------------------------------------------------------------------------------------
// ファイル名: Enemy.h
// 作成者:
// 作成日:
// 説明:
//--------------------------------------------------------------------------------------
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include "Obj3D.h"
#include "CollisionNode.h"

// 自機
class Enemy
{
public:

		enum PARTS_
	{
		PARTS_TANK,		//足
		PARTS_BODY,		//胴体
		PARTS_BODY2,	//胴体２
		PARTS_HAND,		//手
		PARTS_HEAD,		//頭
		PARTS_HAND2,	//手

		PARTS_NUM
	};

	// コンストラクタ
	Enemy(DirectX::Keyboard* keyboard);
	// デストラクタ
	~Enemy();
	// 初期化
	void Initialize();
	// 毎フレーム更新
	void Update();
	// 行列の計算
	void Calc();
	// 描画
	void Draw();
	// 座標を取得
	const DirectX::SimpleMath::Vector3& GetTrans();
	// 回転を取得
	const DirectX::SimpleMath::Vector3& GetRot();
	// 座標を設定
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	// 回転を設定
	void SetRot(const DirectX::SimpleMath::Vector3& rot);
	// ワールド行列を取得
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	
	const SphereNode& GetCollisionEnemy() { return m_CollisionEnemy; }
protected:

	// メンバ変数
	// ロボットの構成パーツ
	std::vector<Obj3d>	m_Obj;
	// キーボード
	DirectX::Keyboard* m_pKeyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;
	// ギミックの稼動サイクル
	float m_cycle;

	//タイマー
	int m_Timer;
	//目標角度
	float m_DistAngle;

	SphereNode m_CollisionEnemy;
};

