#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <directxmath.h>
using namespace DirectX;

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void Update(float fDt);
	void Initialize(int screeWidth, int screenHeight, int screenBarrier);
	void GetViewMatrix(XMMATRIX&);
	void SetPlayerPos(XMFLOAT2 pos) { m_playerPos = pos; }

	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix;
private:
	XMFLOAT2 m_playerPos;
	XMFLOAT2 m_cameraPosGoTo;
	bool m_cameraIsMovingX;
	bool m_cameraIsMovingY;
	float m_cameraVelocity;

	int m_screenWidth;
	int m_screenHeight;
	int m_screenBarrier;
};

#endif