#ifndef _OPTIONS_STATE_H_
#define _OPTIONS_STATE_H_

#include "BaseState.h"
#include "../Utility Files/Locator.h"

class OptionsState : BaseState
{
public:
	OptionsState();
	OptionsState(const OptionsState& other);
	~OptionsState();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	STATE_ACTIONS Update(float fDt);
	void Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool			m_Exit;
	bool			m_GoToBackToMenu;
};

#endif