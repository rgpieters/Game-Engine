#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
	m_playerPos = XMFLOAT2(0.0f, 0.0f);
	m_renderCollsionRects = false;
}

ObjectManager::ObjectManager(const ObjectManager& other)
{
}

ObjectManager::~ObjectManager()
{
}

bool ObjectManager::Initialize()
{
	return true;
}

void ObjectManager::Shutdown()
{
	for (unsigned int i = 0; i < m_objectList.size(); i++)
	{
		m_objectList[i]->Shutdown();
	}

	m_objectList.clear();
}

void ObjectManager::Add(BaseObject* object)
{
	m_objectList.push_back(object);
}

void ObjectManager::Remove(BaseObject* object)
{
	for (vector<BaseObject*>::iterator iter = m_objectList.begin(); iter != m_objectList.end(); iter++)
	{
		if ((*iter) == object)
		{
			(*iter)->Shutdown();
			iter = m_objectList.erase(iter);
			break;
		}
	}
}

void ObjectManager::RemoveAll()
{
	for (unsigned int i = 0; i < m_objectList.size(); i++)
	{
		m_objectList[i]->Shutdown();
	}

	m_objectList.clear();
}

void ObjectManager::Update(float fDt)
{
	for (unsigned int i = 0; i < m_objectList.size(); i++)
	{
		m_objectList[i]->Update(fDt);
		if (m_objectList[i]->GetObjectType() == BaseObject::PLAYER_OBJECT)
		{
			m_playerPos = ((PlayerObject*)m_objectList[i])->GetPosition();
		}
	}

	CheckCollision();
}

void ObjectManager::CheckCollision()
{
	for (unsigned int outerLoop = 0; outerLoop < m_objectList.size(); outerLoop++)
	{
		for (unsigned int innerLoop = 0; innerLoop < m_objectList.size(); innerLoop++)
		{
			if (m_objectList[outerLoop] != m_objectList[innerLoop])
			{
				m_objectList[outerLoop]->CheckCollision(m_objectList[innerLoop]);
			}
		}
	}
}

bool ObjectManager::Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& orthoMatrix)
{
	for (unsigned int i = 0; i < m_objectList.size(); i++)
	{
		m_objectList[i]->Render(deviceContext, viewMatrix, orthoMatrix);
	}

	return true;
}

void ObjectManager::HandleEvent(Event* pEvent)
{
	if (pEvent->GetEventID() == "renderCollisionRects")
	{
		m_renderCollsionRects = !m_renderCollsionRects;

		for (unsigned int i = 0; i < m_objectList.size(); i++)
		{
			//m_objectList[i]->SetRenderCollisionRect(m_renderCollsionRects);
		}
	}
}