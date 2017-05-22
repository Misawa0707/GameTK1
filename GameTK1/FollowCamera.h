/// <summary>
/// ���@�ɒǏ]����J�����N���X
/// </summary>
#pragma once
#include <Windows.h>
#include <Keyboard.h>
#include "Camera.h"

class FollowCamera : public Camera
{
public:
	// ���@�ƃJ�����̋���
	static const float CAMERA_DISTANCE;
	// �R���X�g���N�^
	FollowCamera(int width, int height);
	// �X�V
	void Update() override;

	// �Ǐ]�Ώۂ̍��W���Z�b�g
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);
	// �Ǐ]�Ώۂ̉�]�p���Z�b�g
	void SetTargetAngle(float targetangle);
	//
	void SetKeyboard(DirectX::Keyboard* Keyboard);
protected:
	// �Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;
	// �Ǐ]�Ώۂ̉�]�p
	float m_targetAngle;
	//�L�[�{�[�h
	DirectX::Keyboard* m_Keyboard;
	//�L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;
	//FPS�t���O
	bool isFPS;
};
