#include "PlayerObject.h"
#include "../Utility Files/Locator.h"

PlayerObject::PlayerObject() : BaseObject()
{
	m_objType = PLAYER_OBJECT;
}

PlayerObject::PlayerObject(const PlayerObject& other)
{
}

PlayerObject::~PlayerObject()
{
}

bool PlayerObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, float posX, float posY, char* animationsFile, char* basisShaderFileName, XMFLOAT2 uvs[4],
	float bitmapWidth, float bitmapHeight, OBJECT_TYPE objectType)
{
	return BaseObject::Initialize(device, deviceContext, hwnd, posX, posY, animationsFile, basisShaderFileName, uvs, bitmapWidth, bitmapHeight, objectType, AnimationClass::LOOP);
}

void PlayerObject::Shutdown()
{
	BaseObject::Shutdown();
}

void PlayerObject::Update(float fDt)
{
	m_position.x += m_velocity.x * fDt;
	m_position.y += m_velocity.y * fDt;	

	Locator::GetConsoleWindow().WriteToConsole(m_position.x);
	Locator::GetConsoleWindow().WriteToConsole(m_position.y);

	BaseObject::Update(fDt);
	m_Animations[m_currentAnimation]->PlayAnimation();
}

void PlayerObject::CheckCollision(BaseObject* base)
{
	if (m_AABB->CheckCollision(base->GetAABB()))
	{
		XMFLOAT2 tempMTV = m_AABB->MiniumTranslation(base->GetAABB());

		m_position.x += tempMTV.x;
		m_position.y += tempMTV.y;
	}

	m_Animations[m_currentAnimation]->SetPosition(m_position);
}

void PlayerObject::HandleEvent(Event* pEvent)
{
	if (pEvent->GetEventID() == "playerKeyUp.Up")
	{
		SetVelocityY(0.0);
		ChangeAnimation("Idle");
	}
	else if (pEvent->GetEventID() == "playerKeyUp.Down")
	{
		SetVelocityY(0.0);
		ChangeAnimation("Idle");
	}
	else if (pEvent->GetEventID() == "playerKeyUp.Right")
	{
		SetVelocityX(0.0);
		ChangeAnimation("Idle");
		m_Animations[m_currentAnimation]->SetScale(XMFLOAT2(1.0f, 1.0f));
	}
	else if (pEvent->GetEventID() == "playerKeyUp.Left")
	{
		SetVelocityX(0.0);
		ChangeAnimation("Idle");
		m_Animations[m_currentAnimation]->SetScale(XMFLOAT2(-1.0f, 1.0f));
	}
	else if (pEvent->GetEventID() == "playerKeyDown.Up")
	{
		SetVelocityY(25.0f);
		ChangeAnimation("Walk");
	}
	else if (pEvent->GetEventID() == "playerKeyDown.Down")
	{
		SetVelocityY(-25.0f);
		ChangeAnimation("Walk");
	}
	else if (pEvent->GetEventID() == "playerKeyDown.Right")
	{
		SetVelocityX(25.0f);
		ChangeAnimation("Walk");
		m_Animations[m_currentAnimation]->SetScale(XMFLOAT2(1.0f, 1.0f));
	}
	else if (pEvent->GetEventID() == "playerKeyDown.Left")
	{
		SetVelocityX(-25.0f);
		ChangeAnimation("Walk");
		m_Animations[m_currentAnimation]->SetScale(XMFLOAT2(-1.0f, 1.0f));
	}
}