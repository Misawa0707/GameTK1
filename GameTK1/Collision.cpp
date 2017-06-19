#include "Collision.h"

using namespace DirectX::SimpleMath;

/// <summary>
/// ‹…‚Æ‹…‚Ì‚ ‚½‚è”»’è
/// </summary>
/// <param name="sphereA">‹…A</param>
/// <param name="sphereB">‹…B</param> 
/// <returns>true:“–‚½‚Á‚Ä‚¢‚éfalse:“–‚½‚Á‚Ä‚¢‚È‚¢</returns>
bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	//A‚©‚çB‚ÉŒü‚©‚¤ƒxƒNƒgƒ‹
	Vector3 sub =sphereB.Center - sphereA.Center;
	//A‚ÆB‚Ì‹——£ŒvŽZ
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;
	//”¼Œa‚Ì—Ö
	float radiusSquare = sphereA.Radius + sphereB.Radius;
	radiusSquare = radiusSquare*radiusSquare;
	//‹——£‚ª”¼Œa‚Ì—Ö‚æ‚è‘å‚«‚¯‚ê‚Î“–‚½‚Á‚Ä‚¢‚È‚¢
	if (distanceSquare > radiusSquare)
	{
		return false;
	}
	//“–‚½‚Á‚Ä‚¢‚é
	return true;
}