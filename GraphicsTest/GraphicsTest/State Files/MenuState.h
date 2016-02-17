#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include "BaseState.h"
#include "../Utility Files/Locator.h"

class MenuState : BaseState
{
public:
	enum MENU_INDEX {PLAY, LOAD, OPTIONS, CREDITS, EXIT};
	MenuState();
	MenuState(const MenuState& other);
	~MenuState();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	STATE_ACTIONS Update(float fDt);
	void Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool			m_GoToGameplay;
	bool			m_GoToOptions;
	bool			m_Exit;
	int				m_SelectedIndex;
	XMFLOAT4		m_SelectedColor;
	XMFLOAT4		m_UnSelectedColor;

	void RenderMenu(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
};

#endif