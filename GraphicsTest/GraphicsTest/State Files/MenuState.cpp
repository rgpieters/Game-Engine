#include "MenuState.h"

MenuState::MenuState()
{
	m_State = MENU_STATE;
	m_Exit = false;
	m_GoToGameplay = false;
	m_GoToOptions = false;
	m_SelectedIndex = 0;
	m_SelectedColor = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	m_UnSelectedColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

MenuState::MenuState(const MenuState& other)
{

}

MenuState::~MenuState()
{

}

void MenuState::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight)
{
	BaseState::Initialize(device, deviceContext, screenWidth, screenHeight);
}

void MenuState::Shutdown()
{
	BaseState::Shutdown();
}

BaseState::STATE_ACTIONS MenuState::Update(float fDt)
{
	if (m_Exit)
	{
		return FULL_SHUTDOWN;
	}	
	if (m_GoToOptions)
	{
		m_GoToOptions = false;
		return MOVE_TO_OPTIONS;
	}
	if (m_GoToGameplay)
	{
		return MOVE_TO_GAMEPLAY;
	}	

	return NO_ACTION;
}

void MenuState::Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	m_Camera->Render();
	XMMATRIX tempView;
	m_Camera->GetViewMatrix(tempView);

	m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "MENU", m_Camera->GetPosition().x - 400, m_Camera->GetPosition().y + 268, m_UnSelectedColor);

	RenderMenu(deviceContext, tempView, projectionMatrix);
}

void MenuState::RenderMenu(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	switch (m_SelectedIndex)
	{
		case PLAY:
		{
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "PLAY", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y, m_SelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "LOAD", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 32, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "OPTIONS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 64, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "CREDITS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 96, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "EXIT", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 128, m_UnSelectedColor);
		}
		break;
		case LOAD:
		{
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "PLAY", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "LOAD", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 32, m_SelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "OPTIONS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 64, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "CREDITS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 96, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "EXIT", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 128, m_UnSelectedColor);
		}
		break;
		case OPTIONS:
		{
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "PLAY", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "LOAD", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 32, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "OPTIONS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 64, m_SelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "CREDITS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 96, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "EXIT", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 128, m_UnSelectedColor);
		}
		break;
		case CREDITS:
		{
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "PLAY", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "LOAD", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 32, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "OPTIONS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 64, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "CREDITS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 96, m_SelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "EXIT", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 128, m_UnSelectedColor);
		}
		break;
		case EXIT:
		{
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "PLAY", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "LOAD", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 32, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "OPTIONS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 64, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "CREDITS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 96, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "EXIT", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 128, m_SelectedColor);
		}
		break;
	}
}

LRESULT CALLBACK MenuState::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_INPUT:
		{
			bool keyUp;
			USHORT keyPressed = m_Input->GetInput(lparam, keyUp);

			if (keyPressed == VKEY_ESCAPE)
			{
				m_Exit = true;
			}
			else if (keyPressed == VKEY_RETURN && keyUp)
			{
				switch (m_SelectedIndex)
				{
					case PLAY:
					{
						m_GoToGameplay = true;
					}
					break;
					case LOAD:
					{
						//m_GoToLoad = true;
					}
					break;
					case OPTIONS:
					{
						m_GoToOptions = true;
					}
					break;
					case CREDITS:
					{
						//m_GoToCredits = true;
					}
					break;
					case EXIT:
					{
						m_Exit = true;
					}
					break;
				}
			}
			else if (keyPressed == VKEY_UP && keyUp)
			{
				m_SelectedIndex -= 1;

				if (m_SelectedIndex < 0)
					m_SelectedIndex = 4;
			}
			else if (keyPressed == VKEY_DOWN && keyUp)
			{
				m_SelectedIndex += 1;

				if (m_SelectedIndex > 4)
					m_SelectedIndex = 0;
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