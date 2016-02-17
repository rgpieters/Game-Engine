#include "OptionsState.h"

OptionsState::OptionsState()
{
	m_State = OPTIONS_STATE;
	m_Exit = false;
	m_GoToBackToMenu = false;
}

OptionsState::OptionsState(const OptionsState& other)
{

}

OptionsState::~OptionsState()
{

}

void OptionsState::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight)
{
	BaseState::Initialize(device, deviceContext, screenWidth, screenHeight);
}

void OptionsState::Shutdown()
{
	BaseState::Shutdown();
}

BaseState::STATE_ACTIONS OptionsState::Update(float fDt)
{

	if (m_Exit)
		return FULL_SHUTDOWN;
	if (m_GoToBackToMenu)
		return MOVE_TO_MENU;

	return NO_ACTION;
}

void OptionsState::Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	m_Camera->Render();
	XMMATRIX tempView;
	m_Camera->GetViewMatrix(tempView);

	m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "OPTIONS", m_Camera->GetPosition().x - 400, m_Camera->GetPosition().y + 268, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

LRESULT CALLBACK OptionsState::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
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
			if (keyPressed == VKEY_SPACE && keyUp)
			{
				m_GoToBackToMenu = true;
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