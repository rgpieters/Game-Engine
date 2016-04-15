#include "OptionsState.h"

OptionsState::OptionsState()
{
	m_State = OPTIONS_STATE;
	m_Exit = false;
	m_GoToBackToMenu = false;
	m_GoToPause = false;
	m_SelectedIndex = 0;
	m_SelectedColor = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	m_UnSelectedColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_OptionValues[0] = 100.0;
	m_OptionValues[1] = 100.0;
	m_OptionValues[2] = 100.0;
}

OptionsState::OptionsState(const OptionsState& other)
{

}

OptionsState::~OptionsState()
{

}

void OptionsState::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, STATES prevState)
{
	BaseState::Initialize(device, deviceContext, screenWidth, screenHeight);

	m_PrevState = prevState;
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
	if (m_GoToPause)
		return MOVE_TO_PAUSE;

	return NO_ACTION;
}

void OptionsState::Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	m_Camera->Render();
	XMMATRIX tempView;
	m_Camera->GetViewMatrix(tempView);
	Locator::GetFrustum().ConstructFrustum(projectionMatrix, tempView);

	m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "OPTIONS", m_Camera->GetPosition().x - 400, m_Camera->GetPosition().y + 268, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	char masterBuffer[4];
	char fxBuffer[4];
	char musicBuffer[4];

	itoa(m_OptionValues[MASTER_VOLUME], masterBuffer, 10);
	itoa(m_OptionValues[SOUND_EFFECT_VOLUME], fxBuffer, 10);
	itoa(m_OptionValues[MUSIC_VOLUME], musicBuffer, 10);

	switch (m_SelectedIndex)
	{
		case MASTER_VOLUME:
		{
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "MASTER VOLUME:", m_Camera->GetPosition().x - 400, m_Camera->GetPosition().y + 100, m_SelectedColor);
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "FX VOLUME:", m_Camera->GetPosition().x - 400, m_Camera->GetPosition().y + 68, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "MUSIC VOLUME:", m_Camera->GetPosition().x - 400, m_Camera->GetPosition().y + 36, m_UnSelectedColor);

			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, masterBuffer, m_Camera->GetPosition().x + 125, m_Camera->GetPosition().y + 100, m_SelectedColor);
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, fxBuffer, m_Camera->GetPosition().x + 125, m_Camera->GetPosition().y + 68, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, musicBuffer, m_Camera->GetPosition().x + 125, m_Camera->GetPosition().y + 32, m_UnSelectedColor);
		}
		break;
		case SOUND_EFFECT_VOLUME:
		{
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "MASTER VOLUME:", m_Camera->GetPosition().x - 400, m_Camera->GetPosition().y + 100, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "FX VOLUME:", m_Camera->GetPosition().x - 400, m_Camera->GetPosition().y + 68, m_SelectedColor);
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "MUSIC VOLUME:", m_Camera->GetPosition().x - 400, m_Camera->GetPosition().y + 36, m_UnSelectedColor);

			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, masterBuffer, m_Camera->GetPosition().x + 125, m_Camera->GetPosition().y + 100, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, fxBuffer, m_Camera->GetPosition().x + 125, m_Camera->GetPosition().y + 68, m_SelectedColor);
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, musicBuffer, m_Camera->GetPosition().x + 125, m_Camera->GetPosition().y + 32, m_UnSelectedColor);
		}
		break;
		case MUSIC_VOLUME:
		{
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "MASTER VOLUME:", m_Camera->GetPosition().x - 400, m_Camera->GetPosition().y + 100, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "FX VOLUME:", m_Camera->GetPosition().x - 400, m_Camera->GetPosition().y + 68, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, "MUSIC VOLUME:", m_Camera->GetPosition().x - 400, m_Camera->GetPosition().y + 36, m_SelectedColor);

			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, masterBuffer, m_Camera->GetPosition().x + 125, m_Camera->GetPosition().y + 100, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, fxBuffer, m_Camera->GetPosition().x + 125, m_Camera->GetPosition().y + 68, m_UnSelectedColor);
			m_BitmapFont->Render(deviceContext, tempView, projectionMatrix, musicBuffer, m_Camera->GetPosition().x + 125, m_Camera->GetPosition().y + 32, m_SelectedColor);
		}
		break;
	}
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
				if (m_PrevState == STATES::MENU_STATE)
					m_GoToBackToMenu = true;
				else if (m_PrevState == STATES::PAUSE_STATE)
					m_GoToPause = true;
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
			else if (keyPressed == VKEY_RIGHT)
			{
				m_OptionValues[m_SelectedIndex] += 1.0;

				if (m_OptionValues[m_SelectedIndex] >= 100.0)
					m_OptionValues[m_SelectedIndex] = 100.0;

				switch (m_SelectedIndex)
				{
				case MASTER_VOLUME:
				{
					Locator::GetSoundManager().SetMasterVolume(m_OptionValues[m_SelectedIndex] / 100);
				}
				break;
				case SOUND_EFFECT_VOLUME:
				{
					Locator::GetSoundManager().SetSoundEffectsVolue(m_OptionValues[m_SelectedIndex] / 100);
				}
				break;
				case MUSIC_VOLUME:
				{
					Locator::GetSoundManager().SetBackgroundMusicVolume(m_OptionValues[m_SelectedIndex] / 100);
				}
				break;
				}
			}
			else if (keyPressed == VKEY_LEFT)
			{
				m_OptionValues[m_SelectedIndex] -= 1.0;

				if (m_OptionValues[m_SelectedIndex] <= 0.0)
					m_OptionValues[m_SelectedIndex] = 0.0;

				switch (m_SelectedIndex)
				{
					case MASTER_VOLUME:
					{
						Locator::GetSoundManager().SetMasterVolume(m_OptionValues[m_SelectedIndex] / 100);
					}
					break;
					case SOUND_EFFECT_VOLUME:
					{
						Locator::GetSoundManager().SetSoundEffectsVolue(m_OptionValues[m_SelectedIndex] / 100);
					}
					break;
					case MUSIC_VOLUME:
					{
						Locator::GetSoundManager().SetBackgroundMusicVolume(m_OptionValues[m_SelectedIndex] / 100);
					}
					break;
				}
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