#ifndef _BASE_STATE_H_
#define _BASE_STATE_H_

#include "../Manager Files/InputManager.h"
#include "../Manager Files/ObjectManager.h"
#include "../Event Files/EventManager.h"
#include "../Graphics Files/cameraclass.h"
#include "../Graphics Files/BitmapFontClass.h"

class BaseState
{
public:
	enum STATE_ACTIONS { NO_ACTION, SHUTDOWN_STATE, FULL_SHUTDOWN, MOVE_TO_MENU, MOVE_TO_GAMEPLAY, MOVE_TO_OPTIONS, NUM_ACTIONS };
	enum STATES { ENTRY_STATE, MENU_STATE, OPTIONS_STATE, CREDITS_STATE, GAMEPLAY_STATE, NUM_STATES};

	BaseState();
	BaseState(const BaseState& other);
	~BaseState();

	virtual void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight);
	virtual void Shutdown();
	virtual STATE_ACTIONS Update(float fDt);
	virtual void Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
	virtual LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

	STATES GetState() { return m_State; }

protected:
	ID3D11Device*			m_Device;
	ID3D11DeviceContext*	m_DeviceContext;

	InputManager*			m_Input;
	ObjectManager*			m_ObjectManager;
	EventManager*			m_EventManager;
	CameraClass*			m_Camera;

	BitmapFontClass*		m_BitmapFont;
	STATES					m_State;
};

#endif