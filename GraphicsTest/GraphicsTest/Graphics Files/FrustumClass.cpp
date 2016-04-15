#include "FrustumClass.h"

FrustumClass::FrustumClass()
{
}

FrustumClass::FrustumClass(const FrustumClass& other)
{
}

FrustumClass::~FrustumClass()
{
}

void FrustumClass::Initialize(float screenDepth)
{
	m_screenDepth = screenDepth;
}

void FrustumClass::ConstructFrustum(XMMATRIX& projectionMatrix, XMMATRIX& viewMatrix)
{
	float zMinimum, r;
	XMFLOAT4X4 matrix;
	XMMATRIX projection;
	XMFLOAT4X4 proj;
	XMStoreFloat4x4(&proj, projectionMatrix);

	// Calculate the minimum Z distance in the frustum.
	//zMinimum = -projectionMatrix._43 / projectionMatrix._33;
	//r = m_screenDepth / (m_screenDepth - zMinimum);
	//projectionMatrix._33 = r;
	//projectionMatrix._43 = -r * zMinimum;

	zMinimum = -proj._43 / proj._33;
	r = m_screenDepth / (m_screenDepth - zMinimum);
	proj._33 = r;
	proj._43 = -r * zMinimum;
	projection = XMLoadFloat4x4(&proj);

	// Create the frustum matrix from the view matrix and updated projection matrix.
	XMStoreFloat4x4(&matrix, XMMatrixMultiply(viewMatrix, projectionMatrix));

	XMVECTOR tmp_vector;

	// Calculate near plane of frustum.
	m_planes[0].x = matrix._14 + matrix._13;
	m_planes[0].y = matrix._24 + matrix._23;
	m_planes[0].z = matrix._34 + matrix._33;
	m_planes[0].w = matrix._44 + matrix._43;
	tmp_vector = XMLoadFloat4(&m_planes[0]);
	XMPlaneNormalize(tmp_vector);
	XMStoreFloat4(&m_planes[0], tmp_vector);

	// Calculate far plane of frustum.
	m_planes[1].x = matrix._14 - matrix._13;
	m_planes[1].y = matrix._24 - matrix._23;
	m_planes[1].z = matrix._34 - matrix._33;
	m_planes[1].w = matrix._44 - matrix._43;
	tmp_vector = XMLoadFloat4(&m_planes[1]);
	XMPlaneNormalize(tmp_vector);
	XMStoreFloat4(&m_planes[1], tmp_vector);

	// Calculate left plane of frustum.
	m_planes[2].x = matrix._14 + matrix._11;
	m_planes[2].y = matrix._24 + matrix._21;
	m_planes[2].z = matrix._34 + matrix._31;
	m_planes[2].w = matrix._44 + matrix._41;
	tmp_vector = XMLoadFloat4(&m_planes[2]);
	XMPlaneNormalize(tmp_vector);
	XMStoreFloat4(&m_planes[2], tmp_vector);

	// Calculate right plane of frustum.
	m_planes[3].x = matrix._14 - matrix._11;
	m_planes[3].y = matrix._24 - matrix._21;
	m_planes[3].z = matrix._34 - matrix._31;
	m_planes[3].w = matrix._44 - matrix._41;
	tmp_vector = XMLoadFloat4(&m_planes[3]);
	XMPlaneNormalize(tmp_vector);
	XMStoreFloat4(&m_planes[3], tmp_vector);

	// Calculate top plane of frustum.
	m_planes[4].x = matrix._14 - matrix._12;
	m_planes[4].y = matrix._24 - matrix._22;
	m_planes[4].z = matrix._34 - matrix._32;
	m_planes[4].w = matrix._44 - matrix._42;
	tmp_vector = XMLoadFloat4(&m_planes[4]);
	XMPlaneNormalize(tmp_vector);
	XMStoreFloat4(&m_planes[4], tmp_vector);

	// Calculate bottom plane of frustum.
	m_planes[5].x = matrix._14 + matrix._12;
	m_planes[5].y = matrix._24 + matrix._22;
	m_planes[5].z = matrix._34 + matrix._32;
	m_planes[5].w = matrix._44 + matrix._42;
	tmp_vector = XMLoadFloat4(&m_planes[5]);
	XMPlaneNormalize(tmp_vector);
	XMStoreFloat4(&m_planes[5], tmp_vector);
}

bool FrustumClass::CheckPoint(float x, float y, float z)
{
	for (int i = 0; i < 6; i++)
	{
		XMVECTOR vector = XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat4(&XMFLOAT4(x, y, z, 1.0f)));

		if (vector.m128_f32[0] < 0.0f)
		{
			return false;
		}
	}
	return true;
}

bool FrustumClass::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	for (int i = 0; i < 6; i++)
	{
		if (XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat4(&XMFLOAT4((xCenter - xSize), (yCenter - ySize), (zCenter - zSize), 1.0f))).m128_f32[0] >= 0.0f)
		{
			continue;
		}

		if (XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat4(&XMFLOAT4((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 1.0f))).m128_f32[0] >= 0.0f)
		{
			continue;
		}

		if (XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat4(&XMFLOAT4((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 1.0f))).m128_f32[0] >= 0.0f)
		{
			continue;
		}

		if (XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat4(&XMFLOAT4((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 1.0f))).m128_f32[0] >= 0.0f)
		{
			continue;
		}

		if (XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat4(&XMFLOAT4((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 1.0f))).m128_f32[0] >= 0.0f)
		{
			continue;
		}

		if (XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat4(&XMFLOAT4((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 1.0f))).m128_f32[0] >= 0.0f)
		{
			continue;
		}

		if (XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat4(&XMFLOAT4((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 1.0f))).m128_f32[0] >= 0.0f)
		{
			continue;
		}

		if (XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), XMLoadFloat4(&XMFLOAT4((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 1.0f))).m128_f32[0] >= 0.0f)
		{
			continue;
		}

		return false;
	}
	return true;
}