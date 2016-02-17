#ifndef _OBJECT_MANAGER_H_
#define _OBJECT_MANAGER_H_

#include "..\Object Files\BaseObject.h"
#include "..\Object Files\PlayerObject.h"
#include "..\Event Files\IListener.h"
#include <vector>
using namespace std;

class ObjectManager : IListener
{
private:
	vector<BaseObject*> m_objectList;
	XMFLOAT2 m_playerPos;
	bool m_renderCollsionRects;

	void HandleEvent(Event* pEvent);
	void CheckCollision();
public:
	ObjectManager();
	ObjectManager(const ObjectManager& other);
	~ObjectManager();

	bool Initialize();
	void Shutdown();
	void Add(BaseObject* object);
	void Remove(BaseObject* object);
	void RemoveAll();
	void Update(float fDt);
	bool Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& orthoMatrix);

	XMFLOAT2 GetPlayerPos() { return m_playerPos; }
};

#endif