#ifndef _FRUSTUM_CLASS_H_
#define _FRUSTUM_CLASS_H_

#include <directxmath.h>
using namespace DirectX;

class FrustumClass
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass& other);
	~FrustumClass();

	void Initialize(float screenDepth);

	void ConstructFrustum(XMMATRIX& projectionMatrix, XMMATRIX& viewMatrix);

	bool CheckPoint(float x, float y, float z);
	//bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
	//bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);

private:
	XMFLOAT4 m_planes[6];
	float m_screenDepth;
};

#endif
