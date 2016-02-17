#include "BaseObject.h"

BaseObject::BaseObject()
{
	m_Animation = nullptr;
	m_AABB = nullptr;
	m_position.x = 0;
	m_position.y = 0;
	m_velocity.x = 0;
	m_velocity.y = 0;
	m_objectID = -1;
	m_bitmapWidth = 0;
	m_bitmapHeight = 0;
	m_objType = BASE_OBJECT;

	m_renderCollisionRect = false;
}

BaseObject::BaseObject(const BaseObject& other)
{

}

BaseObject::~BaseObject()
{

}

bool BaseObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, float posX, float posY, char* textureFileName,
				char* basisShaderFileName, XMFLOAT2 uvs[4], float bitmapWidth, float bitmapHeight, OBJECT_TYPE objectType, AnimationClass::ANIMATION_TYPE type)
{
	bool result;

	m_position.x = posX;
	m_position.y = posY;
	m_objType = objectType;

	m_AABB = new AABB();
	m_AABB->Initialize(m_position, bitmapWidth, bitmapHeight);

	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	m_Animation = new AnimationClass();
	if (objectType != COLLISION_OBJECT)
	{
		result = m_Animation->Initialize(device, deviceContext, hwnd, textureFileName, basisShaderFileName, uvs, bitmapWidth, bitmapHeight, type);
		if (!result)
		{
			MessageBox(NULL, L"Could not initialize the bitmap object.", L"Error", MB_OK);
			return false;
		}

		m_Animation->SetPosition(m_position);
	}

	return true;
}

void BaseObject::Shutdown()
{
	if (m_Animation)
	{
		m_Animation->Shutdown();
		delete m_Animation;
		m_Animation = nullptr;
	}

	if (m_AABB)
	{
		delete m_AABB;
		m_AABB = nullptr;
	}

	m_position = XMFLOAT2(0.0, 0.0);
	m_position = XMFLOAT2(0.0, 0.0);
	m_objectID = -1;
	m_bitmapWidth = 0;
	m_bitmapHeight = 0;
}

void BaseObject::Update(float fDt)
{
	if (m_objType != COLLISION_OBJECT)
	{
		m_Animation->Update(fDt);
		m_Animation->SetPosition(m_position);
	}
	m_AABB->Update(m_position);
}

void BaseObject::CheckCollision(BaseObject* base)
{
}

bool BaseObject::Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& orthoMatrix)
{
	if (m_objType != COLLISION_OBJECT)
	{
		m_Animation->Render(deviceContext, viewMatrix, orthoMatrix); // Need to change to take in float
	}
		
	return true;
}