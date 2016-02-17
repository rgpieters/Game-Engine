#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = nullptr;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	XMMATRIX baseViewMatrix;

	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = nullptr;
	}

	return;
}

bool GraphicsClass::Update(float fDt)
{
	return true;
}

void GraphicsClass::RenderStart()
{
	m_D3D->BeginScene(1.0f, 1.0f, 1.0f, 1.0f);

	m_D3D->TurnZBufferOff();
}

bool GraphicsClass::RenderEnd()
{
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	m_D3D->EndScene();

	return true;
}

ID3D11Device* GraphicsClass::GetDevice()
{
	return m_D3D->GetDevice();
}

ID3D11DeviceContext* GraphicsClass::GetDeviceContext()
{
	return m_D3D->GetDeviceContext();
}

XMMATRIX GraphicsClass::GetWorldMatrix()
{
	XMMATRIX worldMatrix;
	m_D3D->GetWorldMatrix(worldMatrix);
	return worldMatrix;
}

XMMATRIX GraphicsClass::GetViewMatrix()
{
	XMMATRIX viewMatrix;

	return viewMatrix;
}

XMMATRIX GraphicsClass::GetOrthoMatrix()
{
	XMMATRIX orthoMatrix;
	m_D3D->GetOrthoMatrix(orthoMatrix);
	return orthoMatrix;
}