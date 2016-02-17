#include "AnimationClass.h"
#include "../Utility Files/Locator.h"

AnimationClass::AnimationClass()
{
	m_Bitmap = NULL;
	m_CurrentFrame = 0;
	m_CurrentDuration = 0.0f;
	m_PlayAnimation = false;
}

AnimationClass::AnimationClass(const AnimationClass& other)
{
}

AnimationClass::~AnimationClass()
{
}

bool AnimationClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, char* animationFileName,
	char* basisShaderFileName, XMFLOAT2 uvs[4], float bitmapWidth, float bitmapHeight, ANIMATION_TYPE type)
{
	bool result;

	string tempFileName;
	LoadAnimationFromFile(animationFileName, tempFileName);

	m_Bitmap = new BitmapClass();
	result = m_Bitmap->Initialize(device, deviceContext, (char*)tempFileName.c_str(), basisShaderFileName, m_FrameList[0].UVs, bitmapWidth, bitmapHeight);
	m_Bitmap->SetPosition(XMFLOAT2(0.0f, 0.0f));

	m_AnimationType = type;

	return result;
}

void AnimationClass::Shutdown()
{
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = NULL;
	}
}

void AnimationClass::Update(float fDt)
{
	if (m_AnimationType == NONE || m_PlayAnimation == false)
		return;

	m_CurrentDuration += fDt; // * m_Speed;

	if (m_CurrentDuration >= m_FrameList[m_CurrentFrame].frameDuration)
	{
		m_CurrentDuration = 0.0f;
		
		switch (m_AnimationType)
		{
			case ONCE:
			{
				if (m_CurrentFrame >= (int)m_FrameList.size())
				{
					m_CurrentFrame = 0;
					m_PlayAnimation = false;
				}
			}
			break;
			case LOOP:
			{
				m_CurrentFrame++;
				if (m_CurrentFrame >= (int)m_FrameList.size())
					m_CurrentFrame = 0;

				Locator::GetConsoleWindow().WriteToConsole(m_CurrentFrame);
			}
			break;
			case OSCILIATING: // this is a reverse loop, not oscilating
			{
				m_CurrentFrame--;
				if (m_CurrentFrame < 0)
					m_CurrentFrame = m_FrameList.size() - 1;
			}
			break;
		}
	}

	m_Bitmap->SetUVs(m_FrameList[m_CurrentFrame].UVs);
	if (m_IsFlipped)
		m_Bitmap->FlipBitmap();
}

void AnimationClass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& orthoMatrix)
{
	m_Bitmap->Render(deviceContext, viewMatrix, orthoMatrix);
}

void AnimationClass::LoadAnimationFromFile(char* animationFileName, string& textureFileName)
{
	string line;
	ifstream file(animationFileName, std::ios::binary);
	float imageWidth;
	float imageHeight;

	Frame tempFrame;

	if (file.is_open())
	{
		// Duration of all frames
		getline(file, line);
		tempFrame.frameDuration = std::stof(line);

		// Image of the animation
		getline(file, line);
		line.resize(line.length() - 1);
		textureFileName = line;

		// Image width and height
		getline(file, line);
		imageWidth = std::stof(line);
		getline(file, line);
		imageHeight = std::stof(line);

		// UVs
		int commaIndex;
		int endIndex;
		char buffer[20];
		float floatX;
		float floatY;

		while (getline(file, line))
		{
			//getline(file, line);
			// TOP LEFT
			commaIndex = line.find(",");
			endIndex = line.find("\r");
			line.copy(buffer, commaIndex, 0);
			floatX = atof(buffer);
			//buffer[0] = 0;
			line.copy(buffer, endIndex - commaIndex, commaIndex + 1);
			floatY = atof(buffer);
			//buffer[0] = 0;
			tempFrame.UVs[BitmapClass::TOP_LEFT] = XMFLOAT2(floatX / imageWidth, floatY / imageHeight);

			// TOP RIGHT
			getline(file, line);
			commaIndex = line.find(",");
			endIndex = line.find("\r");
			line.copy(buffer, commaIndex, 0);
			floatX = atof(buffer);
			//buffer[0] = 0;
			line.copy(buffer, (endIndex - commaIndex), commaIndex + 1);
			//buffer[2] = 0;
			floatY = atof(buffer);
			//buffer[0] = 0;
			tempFrame.UVs[BitmapClass::TOP_RIGHT] = XMFLOAT2(floatX / imageWidth, floatY / imageHeight);

			// BOTTOM LEFT
			getline(file, line);
			commaIndex = line.find(",");
			endIndex = line.find("\r");
			line.copy(buffer, commaIndex, 0);
			floatX = atof(buffer);
			//buffer[0] = 0;
			line.copy(buffer, endIndex - commaIndex, commaIndex + 1);
			floatY = atof(buffer);
			//buffer[0] = 0;
			tempFrame.UVs[BitmapClass::BOTTOM_LEFT] = XMFLOAT2(floatX / imageWidth, floatY / imageHeight);

			// BOTTOM RIGHT
			getline(file, line);
			commaIndex = line.find(",");
			endIndex = line.find("\r");
			line.copy(buffer, commaIndex, 0);
			floatX = atof(buffer);
			//buffer[0] = 0;
			line.copy(buffer, endIndex - commaIndex, commaIndex + 1);
			floatY = atof(buffer);
			//buffer[0] = 0;
			tempFrame.UVs[BitmapClass::BOTTOM_RIGHT] = XMFLOAT2(floatX / imageWidth, floatY / imageHeight);

			m_FrameList.push_back(tempFrame);
		}
	}
}