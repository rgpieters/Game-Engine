#ifndef _PAUSE_STATE_H_
#define _PAUSE_STATE_H_

#include "BaseState.h"
#include "../Utility Files/Locator.h"

class PauseState : BaseState
{
public:
	enum PAUSE_INDEX { RESUME, SAVE, LOAD, OPTIONS, EXIT, NUM_OPTIONS };
	PauseState();
	PauseState(const PauseState& other);
	~PauseState();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, BaseState* gameplayState);
	void Shutdown();
	STATE_ACTIONS Update(float fDt);
	void Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	XMFLOAT4		m_SelectedColor;
	XMFLOAT4		m_UnSelectedColor;
	int				m_SelectedIndex;
	bool			m_Resume;
	bool			m_Save;
	bool			m_Load;
	bool			m_Options;
	bool			m_Exit;

	BaseState*		m_gameplayState;

	void RenderPaused(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
};

#endif