#ifndef _ENTRY_STATE_H_
#define _ENTRY_STATE_H_

#include "BaseState.h"
#include "../Utility Files/Locator.h"

#include "../Graphics Files/AnimationClass.h"

class EntryState : BaseState
{
public:
	EntryState();
	EntryState(const EntryState& other);
	~EntryState();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	STATE_ACTIONS Update(float fDt);
	void Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool			m_Exit;
	bool			m_GotToMenu;
	float			m_PanLevel;
	float			m_MasterVolume;
};

#endif