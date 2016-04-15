#include "PauseState.h"

PauseState::PauseState()
{
	m_State = PAUSE_STATE;
	m_SelectedColor = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	m_UnSelectedColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_gameplayState = NULL;
	m_SelectedIndex = 0;
	m_Resume = false;
	m_Save = false;
	m_Load = false;
	m_Options = false;
	m_Exit = false;
}

PauseState::PauseState(const PauseState& other)
{
}

PauseState::~PauseState()
{
}

void PauseState::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, BaseState* gameplayState)
{
	BaseState::Initialize(device, deviceContext, screenWidth, screenHeight);

	m_gameplayState = gameplayState;
}

void PauseState::Shutdown()
{
	BaseState::Shutdown();
}

BaseState::STATE_ACTIONS PauseState::Update(float fDt)
{
	if (m_Resume)
	{
		m_Resume = false;
		return MOVE_TO_GAMEPLAY;
	}
	else if (m_Save)
	{
		m_Save = false;
		//return MOVE_TO_SAVE;
	}
	else if (m_Load)
	{
		m_Load = false;
		//return MOVE_TO_LOAD;
	}
	else if (m_Options)
	{
		m_Options = false;
		return MOVE_TO_OPTIONS;
	}
	else if (m_Exit)
	{
		return FULL_SHUTDOWN;
	}
	return NO_ACTION;
}

void PauseState::Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	m_Camera->Render();
	XMMATRIX tempView;
	m_Camera->GetViewMatrix(tempView);
	Locator::GetFrustum().ConstructFrustum(projectionMatrix, tempView);

	RenderPaused(deviceContext, tempView, projectionMatrix);
}

void PauseState::RenderPaused(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	m_gameplayState->Render(deviceContext, viewMatrix, projectionMatrix);
	m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "PAUSED", m_Camera->GetPosition().x - 80, m_Camera->GetPosition().y + 200, m_UnSelectedColor);
	switch (m_SelectedIndex)
	{
		case RESUME:
		{
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "RESUME", m_Camera->GetPosition().x - 80, m_Camera->GetPosition().y, m_SelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "SAVE", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 32, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "LOAD", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 64, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "OPTIONS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 96, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "EXIT", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y + - 128, m_UnSelectedColor);
		}
		break;
		case SAVE:
		{
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "RESUME", m_Camera->GetPosition().x - 80, m_Camera->GetPosition().y, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "SAVE", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 32, m_SelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "LOAD", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 64, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "OPTIONS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 96, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "EXIT", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y + -128, m_UnSelectedColor);
		}
		break;
		case LOAD:
		{
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "RESUME", m_Camera->GetPosition().x - 80, m_Camera->GetPosition().y, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "SAVE", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 32, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "LOAD", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 64, m_SelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "OPTIONS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 96, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "EXIT", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y + -128, m_UnSelectedColor);
		}
		break;
		case OPTIONS:
		{
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "RESUME", m_Camera->GetPosition().x - 80, m_Camera->GetPosition().y, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "SAVE", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 32, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "LOAD", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 64, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "OPTIONS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 96, m_SelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "EXIT", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y + -128, m_UnSelectedColor);
		}
		break;
		case EXIT:
		{
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "RESUME", m_Camera->GetPosition().x - 80, m_Camera->GetPosition().y, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "SAVE", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 32, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "LOAD", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y - 64, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "OPTIONS", m_Camera->GetPosition().x - 96, m_Camera->GetPosition().y - 96, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, viewMatrix, projectionMatrix, "EXIT", m_Camera->GetPosition().x - 48, m_Camera->GetPosition().y + -128, m_SelectedColor);
		}
		break;
	}
}

LRESULT PauseState::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_INPUT:
		{
			bool keyUp;
			USHORT keyPressed = m_Input->GetInput(lparam, keyUp);

			if (keyPressed == VKEY_ESCAPE && keyUp)
			{
				m_Resume = true;
			}
			else if (keyPressed == VKEY_UP && keyUp)
			{
				m_SelectedIndex -= 1;

				if (m_SelectedIndex < 0)
					m_SelectedIndex = NUM_OPTIONS - 1;
			}
			else if (keyPressed == VKEY_DOWN && keyUp)
			{
				m_SelectedIndex += 1;

				if (m_SelectedIndex > NUM_OPTIONS - 1)
					m_SelectedIndex = 0;
			}
			else if (keyPressed == VKEY_RETURN && keyUp)
			{
				switch (m_SelectedIndex)
				{
					case RESUME:
					{
						m_Resume = true;
					}
					break;
					case SAVE:
					{
						m_Save = true;
					}	
					break;
					case LOAD:
					{
						m_Load = true;
					}
					break;
					case OPTIONS:
					{
						m_Options = true;
					}
					break;
					case EXIT:
					{
						m_Exit = true;
					}
					break;
				}
			}
		}
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}
