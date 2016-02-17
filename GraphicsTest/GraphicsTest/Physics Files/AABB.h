#ifndef _AABB_H_
#define _AABB_H_

#include <directxmath.h>
using namespace DirectX;

class AABB
{
public:
	AABB();
	~AABB();
	AABB(const AABB& other);

	void Initialize(XMFLOAT2 objectPos, float objectWidth, float objectHeight);
	void Update(XMFLOAT2 objectPos);

	bool CheckCollision(AABB* other);
	XMFLOAT2 MiniumTranslation(AABB* other);
	XMFLOAT2 GetMin() { return m_Min; }
	XMFLOAT2 GetMax() { return m_Max; }
private:

	XMFLOAT2 m_Min;
	XMFLOAT2 m_Max;

	float m_objectWidth;
	float m_objectHeight;
};

#endif
