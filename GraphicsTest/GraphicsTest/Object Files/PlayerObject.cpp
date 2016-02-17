#include "PlayerObject.h"

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

bool PlayerObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, float posX, float posY, char* textureFileName, char* basisShaderFileName, XMFLOAT2 uvs[4],
	float bitmapWidth, float bitmapHeight, OBJECT_TYPE objectType)
{
	return BaseObject::Initialize(device, deviceContext, hwnd, posX, posY, textureFileName, basisShaderFileName, uvs, bitmapWidth, bitmapHeight, objectType, AnimationClass::LOOP);
}

void PlayerObject::Shutdown()
{
	BaseObject::Shutdown();
}

void PlayerObject::Update(float fDt)
{
	m_position.x += m_velocity.x * fDt;
	m_position.y += m_velocity.y * fDt;	

	BaseObject::Update(fDt);
	m_Animation->PlayAnimation();
}

void PlayerObject::CheckCollision(BaseObject* base)
{
	if (m_AABB->CheckCollision(base->GetAABB()))
	{
		XMFLOAT2 tempMTV = m_AABB->MiniumTranslation(base->GetAABB());

		m_position.x += tempMTV.x;
		m_position.y += tempMTV.y;
	}

	m_Animation->SetPosition(m_position);
}

void PlayerObject::HandleEvent(Event* pEvent)
{
	if (pEvent->GetEventID() == "playerKeyUp.Up")
	{
		SetVelocityY(0.0);
		m_Animation->StopAnimation();
	}
	else if (pEvent->GetEventID() == "playerKeyUp.Down")
	{
		SetVelocityY(0.0);
		m_Animation->StopAnimation();
	}
	else if (pEvent->GetEventID() == "playerKeyUp.Right")
	{
		SetVelocityX(0.0);
		m_Animation->StopAnimation();
	}
	else if (pEvent->GetEventID() == "playerKeyUp.Left")
	{
		SetVelocityX(0.0);
		m_Animation->StopAnimation();
	}
	else if (pEvent->GetEventID() == "playerKeyDown.Up")
	{
		SetVelocityY(25.0f);
		m_Animation->PlayAnimation();
	}
	else if (pEvent->GetEventID() == "playerKeyDown.Down")
	{
		SetVelocityY(-25.0f);
		m_Animation->PlayAnimation();
	}
	else if (pEvent->GetEventID() == "playerKeyDown.Right")
	{
		SetVelocityX(25.0f);
		//m_Animation->SetIsFlipped(false);
		m_Animation->SetScale(XMFLOAT2(1.0f, 1.0f));
		m_Animation->SetType(AnimationClass::LOOP);
		m_Animation->PlayAnimation();
	}
	else if (pEvent->GetEventID() == "playerKeyDown.Left")
	{
		SetVelocityX(-25.0f);
		//m_Animation->SetIsFlipped(true);
		m_Animation->SetScale(XMFLOAT2(-1.0f, 1.0f));
		m_Animation->SetType(AnimationClass::OSCILIATING);
		m_Animation->PlayAnimation();
	}
}