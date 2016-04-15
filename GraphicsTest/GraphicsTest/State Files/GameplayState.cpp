#include "GameplayState.h"
#include "../Utility Files/Locator.h"
GameplayState::GameplayState()
{
	m_State = GAMEPLAY_STATE;
	m_Paused = false;
	m_TileMap = NULL;
	m_PlayerObject = NULL;
}

GameplayState::GameplayState(const GameplayState& other)
{

}

GameplayState::~GameplayState()
{

}

void GameplayState::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,  HWND hwnd, int screenWidth, int screenHeight)
{
	Locator::GetConsoleWindow().WriteToConsole("Gameplay State Enter");
	BaseState::Initialize(device, deviceContext, screenWidth, screenHeight);

	m_TileMap = new TileMap();
	m_TileMap->Initialize(device, deviceContext, hwnd, screenWidth, screenHeight);
	for (unsigned int i = 0; i < m_TileMap->GetObjectList().size(); i++)
	{
		m_ObjectManager->Add(m_TileMap->GetObjectList()[i]);
	}

	XMFLOAT2 uvs[4];
	uvs[BitmapClass::TOP_LEFT] = XMFLOAT2(0.0f, 0.0f);
	uvs[BitmapClass::TOP_RIGHT] = XMFLOAT2(0.125f, 0.0f);
	uvs[BitmapClass::BOTTOM_LEFT] = XMFLOAT2(0.0f, 0.5f);
	uvs[BitmapClass::BOTTOM_RIGHT] = XMFLOAT2(0.125f, 0.5f);

	m_PlayerObject = new PlayerObject();
	m_PlayerObject->Initialize(device, deviceContext, hwnd, 0, 0, "Resource Files/Animations/MegaMan_Animations.txt", "Resource Files/myColor", uvs, 64, 64, BaseObject::PLAYER_OBJECT);
	m_ObjectManager->Add((BaseObject*)m_PlayerObject);

	m_EventManager->RegisterClient("playerKeyUp.Up", (IListener*)m_PlayerObject);
	m_EventManager->RegisterClient("playerKeyUp.Down", (IListener*)m_PlayerObject);
	m_EventManager->RegisterClient("playerKeyUp.Right", (IListener*)m_PlayerObject);
	m_EventManager->RegisterClient("playerKeyUp.Left", (IListener*)m_PlayerObject);
	m_EventManager->RegisterClient("playerKeyDown.Up", (IListener*)m_PlayerObject);
	m_EventManager->RegisterClient("playerKeyDown.Down", (IListener*)m_PlayerObject);
	m_EventManager->RegisterClient("playerKeyDown.Right", (IListener*)m_PlayerObject);
	m_EventManager->RegisterClient("playerKeyDown.Left", (IListener*)m_PlayerObject);
}

void GameplayState::Shutdown()
{
	Locator::GetConsoleWindow().WriteToConsole("Gameplay State Exit");
	if (m_TileMap)
	{
		m_TileMap->Shutdown();
		delete m_TileMap;
		m_TileMap = NULL;
	}
	if (m_PlayerObject)
	{
		m_PlayerObject->Shutdown();
		delete m_PlayerObject;
		m_PlayerObject = NULL;
	}
}

BaseState::STATE_ACTIONS GameplayState::Update(float fDt)
{
	if (m_Paused)
	{
		m_Paused = false;
		return MOVE_TO_PAUSE;
	}

	BaseState::Update(fDt);
	m_Camera->SetPlayerPos(m_PlayerObject->GetPosition());
	m_Camera->Update(fDt);

	return NO_ACTION;
}

void GameplayState::Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	m_Camera->Render();
	XMMATRIX tempView;
	m_Camera->GetViewMatrix(tempView);
	Locator::GetFrustum().ConstructFrustum(projectionMatrix, tempView);

	m_TileMap->BackgroundForegroundRender(deviceContext, tempView, projectionMatrix);
	m_ObjectManager->Render(deviceContext, tempView, projectionMatrix);
	m_TileMap->TopRender(deviceContext, tempView, projectionMatrix);
	m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "GAMEPLAY", m_Camera->GetPosition().x - 400, m_Camera->GetPosition().y + 268, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

LRESULT CALLBACK GameplayState::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_INPUT:
		{
			bool keyUp;
			USHORT keyPressed = m_Input->GetInput(lparam, keyUp);

			if (keyPressed == VKEY_ESCAPE && keyUp)
			{
				m_Paused = true;
			}
			if (keyPressed == VKEY_UP && keyUp)
			{
				m_EventManager->SendEvent("playerKeyUp.Up");
			}
			else if (keyPressed == VKEY_UP && !keyUp)
			{
				m_EventManager->SendEvent("playerKeyDown.Up");
			}
			else if (keyPressed == VKEY_DOWN && keyUp)
			{
				m_EventManager->SendEvent("playerKeyUp.Down");
			}
			else if (keyPressed == VKEY_DOWN && !keyUp)
			{
				m_EventManager->SendEvent("playerKeyDown.Down");
			}
			else if (keyPressed == VKEY_RIGHT && keyUp)
			{
				m_EventManager->SendEvent("playerKeyUp.Right");
			}
			else if (keyPressed == VKEY_RIGHT && !keyUp)
			{
				m_EventManager->SendEvent("playerKeyDown.Right");
			}
			else if (keyPressed == VKEY_LEFT && keyUp)
			{
				m_EventManager->SendEvent("playerKeyUp.Left");
			}
			else if (keyPressed == VKEY_LEFT && !keyUp)
			{
				m_EventManager->SendEvent("playerKeyDown.Left");
			}
			return 0;
		}
		break;
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}