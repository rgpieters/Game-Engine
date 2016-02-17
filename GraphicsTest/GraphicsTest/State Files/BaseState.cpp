#include "BaseState.h"

BaseState::BaseState()
{
	m_Input = NULL;
	m_ObjectManager = NULL;
	m_EventManager = NULL;
	m_Camera = NULL;
	m_BitmapFont = NULL;
}

BaseState::BaseState(const BaseState& other)
{

}

BaseState::~BaseState()
{

}

void BaseState::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight)
{
	m_Device = device;
	m_DeviceContext = deviceContext;

	m_Input = new InputManager();

	m_ObjectManager = new ObjectManager();
	m_ObjectManager->Initialize();

	m_Camera = new CameraClass();
	m_Camera->Initialize(screenWidth, screenHeight, 100);

	m_EventManager = new EventManager();
	m_EventManager->Initialize();

	m_BitmapFont = new BitmapFontClass();
	m_BitmapFont->Initialize(device, deviceContext, "Resource Files/font1.png", "Resource Files/myColor");
	m_BitmapFont->SetPosition(XMFLOAT2(0.0f, 0.0f));
}

void BaseState::Shutdown()
{
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = NULL;
	}
	if (m_ObjectManager)
	{
		m_ObjectManager->Shutdown();
		delete m_ObjectManager;
		m_ObjectManager = NULL;
	}
	if (m_EventManager)
	{
		m_EventManager->ClearEvents();
		delete m_EventManager;
		m_EventManager = NULL;
	}
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = NULL;
	}
	if (m_BitmapFont)
	{
		m_BitmapFont->Shutdown();
		delete m_BitmapFont;
		m_BitmapFont = NULL;
	}
}

BaseState::STATE_ACTIONS BaseState::Update(float fDt)
{
	m_ObjectManager->Update(fDt);
	m_Camera->Update(fDt);
	m_EventManager->ProcessEvents();

	return NO_ACTION;
}

void BaseState::Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	
}

LRESULT CALLBACK BaseState::MessageHandler(HWND, UINT, WPARAM, LPARAM)
{
	return 0;
}