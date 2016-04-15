#include "EntryState.h"
#include "../Utility Files/Locator.h"

EntryState::EntryState()
{
	m_State = ENTRY_STATE;
	m_Exit = false;
	m_GotToMenu = false;

	m_PanLevel = 0.0f;
	m_MasterVolume = 1.0f;
}

EntryState::EntryState(const EntryState& other)
{

}

EntryState::~EntryState()
{

}

void EntryState::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight)
{
	BaseState::Initialize(device, deviceContext, screenWidth, screenHeight);
	Locator::GetSoundManager().LoadSoundFile("Resource Files/Sounds/Peppy--The-Firing-Squad_YMXB-320.mp3", "background", FMOD_LOOP_NORMAL);
	//Locator::GetSoundManager().PlaySoundFile("background", SoundManager::BACKGROUND_MUSIC);
	Locator::GetSoundManager().LoadSoundFile("Resource Files/Sounds/beep-07.mp3", "beepSound");
}

void EntryState::Shutdown()
{
	BaseState::Shutdown();
}

BaseState::STATE_ACTIONS EntryState::Update(float fDt)
{
	if (m_GotToMenu)
	{
		//Locator::GetSoundManager().PlaySoundFile("beepSound", SoundManager::SOUND_EFFECTS);
		return MOVE_TO_MENU;
	}
		
	if (m_Exit)
		return FULL_SHUTDOWN;

	return NO_ACTION;
}

void EntryState::Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	m_Camera->Render();
	XMMATRIX tempView;
	m_Camera->GetViewMatrix(tempView);
	Locator::GetFrustum().ConstructFrustum(projectionMatrix, tempView);

	m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "ENTRY", m_Camera->GetPosition().x - 400, m_Camera->GetPosition().y + 268, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "PRESS SPACE TO CONTINUE", m_Camera->GetPosition().x - 365, m_Camera->GetPosition().y, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

LRESULT CALLBACK EntryState::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
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
				m_GotToMenu = true;
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