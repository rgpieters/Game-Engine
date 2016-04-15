#ifndef _GAMEPLAY_STATE_H_
#define _GAMEPLAY_STATE_H_

#include "BaseState.h"
#include "../Utility Files/Locator.h"
#include "../TileMap Files/TileMap.h"
#include "../Object Files/PlayerObject.h"

class GameplayState : BaseState
{
public:
	GameplayState();
	GameplayState(const GameplayState& other);
	~GameplayState();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	STATE_ACTIONS Update(float fDt);
	void Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool			m_Paused;
	TileMap*		m_TileMap;
	PlayerObject*	m_PlayerObject;
};

#endif