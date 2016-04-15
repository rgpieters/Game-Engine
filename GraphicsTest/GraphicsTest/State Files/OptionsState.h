#ifndef _OPTIONS_STATE_H_
#define _OPTIONS_STATE_H_

#include "BaseState.h"
#include "../Utility Files/Locator.h"

class OptionsState : BaseState
{
public:
	enum OPTIONS_INDEX { MASTER_VOLUME, SOUND_EFFECT_VOLUME, MUSIC_VOLUME, NUM_OPTIONS };
	OptionsState();
	OptionsState(const OptionsState& other);
	~OptionsState();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, STATES prevState);
	void Shutdown();
	STATE_ACTIONS Update(float fDt);
	void Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool			m_Exit;
	bool			m_GoToBackToMenu;
	bool			m_GoToPause;
	int				m_SelectedIndex;
	XMFLOAT4		m_SelectedColor;
	XMFLOAT4		m_UnSelectedColor;
	float			m_OptionValues[NUM_OPTIONS];
	STATES			m_PrevState;
};

#endif