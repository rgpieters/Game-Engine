#include "AABB.h"

AABB::AABB()
{

}

AABB::~AABB()
{

}

AABB::AABB(const AABB& other)
{

}

void AABB::Initialize(XMFLOAT2 objectPos, float objectWidth, float objectHeight)
{
	m_objectWidth = objectWidth;
	m_objectHeight = objectHeight;

	m_Min.x = objectPos.x;
	m_Min.y = objectPos.y - objectHeight;
	m_Max.x = objectPos.x + objectWidth;
	m_Max.y = objectPos.y + objectHeight;
}

void AABB::Update(XMFLOAT2 objectPos)
{
	m_Min.x = objectPos.x;
	m_Min.y = objectPos.y - m_objectHeight;
	m_Max.x = objectPos.x + m_objectWidth;
	m_Max.y = objectPos.y;
}

bool AABB::CheckCollision(AABB* other)
{
	if (m_Max.x < other->GetMin().x ||
		m_Max.y < other->GetMin().y ||
		m_Min.x > other->GetMax().x ||
		m_Min.y > other->GetMax().y)
	{
		return false;
	}

	return true;
}

XMFLOAT2 AABB::MiniumTranslation(AABB* other)
{
	XMFLOAT2 thisMin = m_Min;
	XMFLOAT2 thisMax = m_Max;
	XMFLOAT2 otherMin = other->GetMin();
	XMFLOAT2 otherMax = other->GetMax();

	XMFLOAT2 MTV;
	
	float left = otherMin.x - thisMax.x;
	float right = otherMax.x - thisMin.x;
	float top = otherMin.y - thisMax.y;
	float bottom = otherMax.y - thisMin.y;

	if (left > 0 || right < 0)
	{
		// Insert message box here, AABBs don't intersect!
	}
	if (top > 0 || bottom < 0)
	{
		// Insert message box here, AABBs don't intersect!
	}

	if (abs(left) < right)
		MTV.x = left;
	else
		MTV.x = right;

	if (abs(top) < bottom)
		MTV.y = top;
	else
		MTV.y = bottom;

	if (abs(MTV.x) < abs(MTV.y))
		MTV.y = 0;
	else
		MTV.x = 0;

	return MTV;
}