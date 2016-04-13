#include "BaseObject.h"

BaseObject::BaseObject()
{
	m_currentAnimation = 0;
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

bool BaseObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, float posX, float posY, char* animationsFile,
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

	AnimationClass* tempAnimation;


	if (objectType != COLLISION_OBJECT)
	{
		string line;
		ifstream file(animationsFile, std::ios::binary);

		while (file.is_open())
		{
			getline(file, line);

			int endIndex = line.find("\r");
			char buffer[100];
			memset(&buffer[0], 0, sizeof(buffer));
			line.copy(buffer, endIndex , 0);

			tempAnimation = new AnimationClass();

			result = tempAnimation->Initialize(device, deviceContext, hwnd, buffer, basisShaderFileName, uvs, bitmapWidth, bitmapHeight, type);
			if (!result)
			{
				MessageBox(NULL, L"Could not initialize an animation object.", L"Error", MB_OK);
				return false;
			}

			m_Animations.push_back(tempAnimation);

			if (file.eof())
				break;
		}
	}
	else
	{
		tempAnimation = new AnimationClass();
		m_Animations.push_back(tempAnimation);
	}

	return true;
}

void BaseObject::Shutdown()
{
	if (!m_Animations.empty())
	{
		for (unsigned int i = 0; i < m_Animations.size(); i++)
		{
			m_Animations[i]->Shutdown();
			delete m_Animations[i];
			m_Animations[i] = nullptr;
		}

		m_Animations.clear();
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
		m_Animations[m_currentAnimation]->Update(fDt);
		m_Animations[m_currentAnimation]->SetPosition(m_position);
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
		m_Animations[m_currentAnimation]->Render(deviceContext, viewMatrix, orthoMatrix); // Need to change to take in float
	}
		
	return true;
}

void BaseObject::ChangeAnimation(string newAnimation)
{
	for (unsigned int i = 0; i < m_Animations.size(); i++)
	{
		if (m_Animations[i]->GetName() == newAnimation)
		{
			if (m_currentAnimation == i)
				return;
			m_Animations[m_currentAnimation]->StopAnimation();
			m_currentAnimation = i;
			m_Animations[m_currentAnimation]->SetPosition(m_position);
			m_Animations[m_currentAnimation]->PlayAnimation();
			return;
		}
	}


}