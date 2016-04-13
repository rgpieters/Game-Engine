#ifndef _BASE_OBJECT_H_
#define _BASE_OBJECT_H_

#include "../Graphics Files/AnimationClass.h"
#include "../Physics Files/AABB.h"

class BaseObject
{
public:	
	enum OBJECT_TYPE
	{
		BASE_OBJECT,
		PLAYER_OBJECT,
		COLLISION_OBJECT,
		NUM_OBJECTS
	};

	BaseObject();
	BaseObject(const BaseObject&);
	~BaseObject();

	virtual bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, float posX, float posY, char* animationsFile, char* basisShaderFileName, XMFLOAT2 uvs[4],
		float bitmapWidth, float bitmapHeight, OBJECT_TYPE objectType, AnimationClass::ANIMATION_TYPE type = AnimationClass::NONE);
	virtual void Shutdown();
	virtual void Update(float);
	bool Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& orthoMatrix);
	void ChangeAnimation(string newAnimation);

	virtual void CheckCollision(BaseObject* base);

	void SetID(int objectID) { m_objectID = objectID; }
	void SetVelocityY(float velY) { m_velocity.y = velY; }
	void SetVelocityX(float velX) { m_velocity.x = velX; }
	OBJECT_TYPE GetObjectType() { return m_objType; }
	
	void SetPosition(XMFLOAT2 pos) { m_position = pos; }
	XMFLOAT2 GetPosition() { return m_position; }
	AABB* GetAABB() { return m_AABB; }

protected:
	XMFLOAT2 m_position;
	XMFLOAT2 m_velocity;
	float m_bitmapWidth;
	float m_bitmapHeight;
	unsigned int m_objectID;
	OBJECT_TYPE m_objType;
	vector<AnimationClass*> m_Animations;
	int m_currentAnimation;

	bool m_renderCollisionRect;

	AABB* m_AABB;
};

#endif