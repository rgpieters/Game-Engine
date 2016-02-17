#include "cameraclass.h"

CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_playerPos = XMFLOAT2(0.0f, 0.0f);
	m_cameraPosGoTo = XMFLOAT2(0.0f, 0.0f);

	m_cameraIsMovingX = false;
	m_cameraIsMovingY = false;
	m_cameraVelocity = 75.0f;// try 200

	m_screenWidth = 0;
	m_screenHeight = 0;
	m_screenBarrier = 0;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}

void CameraClass::Initialize(int screeWidth, int screenHeight, int screenBarrier)
{
	m_screenWidth = screeWidth;
	m_screenHeight = screenHeight;
	m_screenBarrier = screenBarrier;
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}


XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Update(float fDt)
{
	if (m_cameraIsMovingX)
	{
		m_positionX += m_cameraVelocity * fDt;

		if (m_positionX >= m_cameraPosGoTo.x && m_cameraVelocity > 0.0f)
		{
			m_cameraIsMovingX = false;
		}
		else if (m_positionX <= m_cameraPosGoTo.x && m_cameraVelocity < 0.0f)
		{
			m_cameraIsMovingX = false;
		}
	}
	else if (m_cameraIsMovingY)
	{
		m_positionY += m_cameraVelocity * fDt;

		if (m_positionY >= m_cameraPosGoTo.y && m_cameraVelocity > 0.0f)
		{
			m_cameraIsMovingY = false;
		}
		else if (m_positionY <= m_cameraPosGoTo.y && m_cameraVelocity < 0.0f)
		{
			m_cameraIsMovingY = false;
		}
	}
}

void CameraClass::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	float playerX = m_playerPos.x;
	float playerY = m_playerPos.y;

	if (playerX >= (m_positionX + (m_screenWidth / 2) - m_screenBarrier)) // (ScreenWidth / 2) - screenBarrier
	{
		m_cameraIsMovingX = true;
		m_cameraPosGoTo.x = playerX;
		if (m_cameraVelocity < 0.0f)
		{
			m_cameraVelocity *= -1.0f;
		}
	}
	else if (playerX <= (m_positionX - (m_screenWidth / 2) + m_screenBarrier))
	{
		m_cameraIsMovingX = true;
		m_cameraPosGoTo.x = playerX;
		if (m_cameraVelocity > 0.0f)
		{
			m_cameraVelocity *= -1.0f;
		}
	}

	if (playerY >= (m_positionY + (m_screenHeight / 2) - m_screenBarrier)) // (ScreenHeight / 2 ) - screenBarrier
	{
		m_cameraIsMovingY = true;
		m_cameraPosGoTo.y = playerY;
		if (m_cameraVelocity < 0.0f)
		{
			m_cameraVelocity *= -1.0f;
		}
	}
	else if (playerY <= (m_positionY - (m_screenHeight / 2) + m_screenBarrier))
	{
		m_cameraIsMovingY = true;
		m_cameraPosGoTo.y = playerY;
		if (m_cameraVelocity > 0.0f)
		{
			m_cameraVelocity *= -1.0f;
		}
	}

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw   = m_rotationY * 0.0174532925f;
	roll  = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}