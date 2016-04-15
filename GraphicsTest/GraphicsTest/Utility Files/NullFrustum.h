#ifndef _NULL_FRUSTUM_H_
#define _NULL_FRUSTUM_H_

#include "../Graphics Files/FrustumClass.h"

class NullFrustum : public FrustumClass
{
	virtual void Initialize(float screenDepth) {};
	virtual void ConstructFrustum(XMMATRIX& projectionMatrix, XMMATRIX& viewMatrix) {};
	virtual void CheckPoint(float x, float y, float z) {};
	virtual void CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize) {};
};

#endif
