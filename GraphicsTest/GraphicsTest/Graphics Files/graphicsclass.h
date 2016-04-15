#ifndef _GRAPHICS_CLASS_H_
#define _GRAPHICS_CLASS_H_

#include <Windows.h>
#include "d3dclass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Update(float fDt);

	void RenderStart();
	bool RenderEnd();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	XMMATRIX GetWorldMatrix();
	XMMATRIX GetViewMatrix();
	XMMATRIX GetOrthoMatrix();

private:
	D3DClass*		m_D3D;
};

#endif