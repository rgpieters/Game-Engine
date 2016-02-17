#ifndef _ANIMATION_CLASS_H_
#define _ANIMATION_CLASS_H_

#include "BitmapClass.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

class AnimationClass
{
public:
	enum ANIMATION_TYPE { NONE, ONCE, LOOP, OSCILIATING, NUM_TYPES};
	AnimationClass();
	AnimationClass(const AnimationClass& other);
	~AnimationClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, char* animationFileName,
		char* basisShaderFileName, XMFLOAT2 uvs[4], float bitmapWidth, float bitmapHeight, ANIMATION_TYPE type = NONE);
	void Shutdown();
	void Update(float fDt);
	void Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& orthoMatrix);

	void SetPosition(XMFLOAT2 pos)		{ m_Bitmap->SetPosition(pos); }
	void SetScale(XMFLOAT2 scale)		{ m_Bitmap->SetScale(scale); }
	void SetRotation(float rot)			{ m_Bitmap->SetRotation(rot); }
	void SetType(ANIMATION_TYPE type)	{ m_AnimationType = type; }
	void PlayAnimation()				{ m_PlayAnimation = true; }
	void StopAnimation()				{ m_PlayAnimation = false; }
	void SetIsFlipped(bool flipped)		{ m_IsFlipped = flipped; }
private:
	struct Frame
	{
		XMFLOAT2 UVs[4];
		float frameDuration;
	};

	int m_CurrentFrame;
	float m_CurrentDuration;
	float m_Speed;
	bool m_PlayAnimation;
	bool m_IsFlipped;
	ANIMATION_TYPE m_AnimationType;
	BitmapClass* m_Bitmap;
	std::vector<Frame> m_FrameList;

	void LoadAnimationFromFile(char* animationFileName, string& textureFileName);
};

#endif