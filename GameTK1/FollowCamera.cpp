
#include "FollowCamera.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

// 静的メンバ変数の初期化
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
		//カメラのフラグ切り替え
		isFPS = !isFPS;
	}
	if (m_player)
	{
		//追従対象の座標と角度をセット
		SetTargetPos(m_player->GetTrans());
		SetTargetAngle(m_player->GetRot().y);
	}
	// カメラ視点座標、参照点座標
	Vector3 eyepos, refpos;


	if(isFPS)
	{//FPSカメラの処理
	 // 参照点座標を計算
		eyepos = m_targetPos + Vector3(0, 0.2f, 0);
		// 自機からカメラ座標への差分
		Vector3 cameraV(0, 0, -CAMERA_DISTANCE);
		// 自機の後ろに回り込む為の回転行列
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		// カメラへのベクトルを回転
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		// カメラ座標を計算
		refpos = eyepos + cameraV;
		
	}
	else
	{//TPSカメラの処理
		// 参照点座標を計算
		refpos = m_targetPos + Vector3(0, 2, 0);
		// 自機からカメラ座標への差分
		Vector3 cameraV(0, 0, CAMERA_DISTANCE);
		// 自機の後ろに回り込む為の回転行列
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		// カメラへのベクトルを回転
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		// カメラ座標を計算
		eyepos = refpos + cameraV;
	
	//視点を現在位置から補間する
	eyepos = m_eyepos + (eyepos- m_eyepos)*0.05f;
	//参照点を現在地から補間する
	refpos = m_refpos + (refpos - m_refpos)*0.2f;
	}

	SetEyePos(eyepos);
	SetRefPos(refpos);

	// 基底クラスの更新
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
