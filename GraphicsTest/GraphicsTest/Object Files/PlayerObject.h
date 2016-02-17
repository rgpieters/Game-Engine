#ifndef _PLAYER_OBJECT_H_
#define _PLAYER_OBJECT_H_

#include "BaseObject.h"
#include "../Event Files/IListener.h"

class PlayerObject : public BaseObject, IListener
{
public:
	PlayerObject();
	PlayerObject(const PlayerObject& other);
	~PlayerObject();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, float posX, float posY, char* textureFileName, char* basisShaderFileName, XMFLOAT2 uvs[4],
		float bitmapWidth, float bitmapHeight, OBJECT_TYPE objectType);
	void Shutdown();
	void Update(float fDt);
	void CheckCollision(BaseObject* base);
private:
	void HandleEvent(Event* pEvent);
};

#endif