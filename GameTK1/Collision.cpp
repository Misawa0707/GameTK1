#include "Collision.h"

using namespace DirectX::SimpleMath;

/// <summary>
/// 球と球のあたり判定
/// </summary>
/// <param name="sphereA">球A</param>
/// <param name="sphereB">球B</param>
/// <returns>true:当たっているfalse:当たっていない</returns>
bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	//AからBに向かうベクトル
	Vector3 sub =sphereB.Center - sphereA.Center;
	//AとBの距離計算
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;
	//半径の輪
	float radiusSquare = sphereA.Radius + sphereB.Radius;
	radiusSquare = radiusSquare*radiusSquare;
	//距離が半径の輪より大きければ当たっていない
	if (distanceSquare > radiusSquare)
	{
		return false;
	}
	//当たっている
	return true;
}