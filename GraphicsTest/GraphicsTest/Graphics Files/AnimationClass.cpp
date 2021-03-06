#include "AnimationClass.h"
#include "../Utility Files/Locator.h"

AnimationClass::AnimationClass()
{
	m_Bitmap = NULL;
	m_CurrentFrame = 0;
	m_CurrentDuration = 0.0f;
	m_PlayAnimation = false;
	m_OscillatingUpwards = true;
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

	m_CurrentDuration += fDt * m_Speed;

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
			}
			break;
			case REVERSE:
			{
				m_CurrentFrame--;
				if (m_CurrentFrame < 0)
					m_CurrentFrame = m_FrameList.size() - 1;
			}
			break;
			case OSCILLATING:
			{
				if (m_OscillatingUpwards)
				{
					m_CurrentFrame++;
					if (m_CurrentFrame >= (int)m_FrameList.size())
					{
						m_CurrentFrame = m_FrameList.size() - 1;
						m_OscillatingUpwards = false;
					}
				}
				else
				{
					m_CurrentFrame--;
					if (m_CurrentFrame < 0)
					{
						m_CurrentFrame = 0;
						m_OscillatingUpwards = true;
					}
				}
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
		// Name of Animation
		getline(file, line);
		char buffers[10];
		int rIndex = line.find("\r");
		memset(&buffers[0], 0, sizeof(buffers));
		line.copy(buffers, rIndex, 0);
		m_AnimationName = buffers;

		// Speed of the animation
		getline(file, line);
		m_Speed = std::stof(line);

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
			// Frame Duration
			endIndex = line.find("\r");
			line.copy(buffer, endIndex, 0);
			tempFrame.frameDuration = atof(buffer);
			memset(&buffer[0], 0, sizeof(buffer));

			// TOP LEFT
			getline(file, line);
			commaIndex = line.find(",");
			endIndex = line.find("\r");
			line.copy(buffer, commaIndex, 0);
			floatX = atof(buffer);
			line.copy(buffer, endIndex - commaIndex, commaIndex + 1);
			floatY = atof(buffer);
			memset(&buffer[0], 0, sizeof(buffer));
			tempFrame.UVs[BitmapClass::TOP_LEFT] = XMFLOAT2(floatX / imageWidth, floatY / imageHeight);

			// TOP RIGHT
			getline(file, line);
			commaIndex = line.find(",");
			endIndex = line.find("\r");
			line.copy(buffer, commaIndex, 0);
			floatX = atof(buffer);
			line.copy(buffer, (endIndex - commaIndex), commaIndex + 1);
			floatY = atof(buffer);
			memset(&buffer[0], 0, sizeof(buffer));
			tempFrame.UVs[BitmapClass::TOP_RIGHT] = XMFLOAT2(floatX / imageWidth, floatY / imageHeight);

			// BOTTOM LEFT
			getline(file, line);
			commaIndex = line.find(",");
			endIndex = line.find("\r");
			line.copy(buffer, commaIndex, 0);
			floatX = atof(buffer);
			line.copy(buffer, endIndex - commaIndex, commaIndex + 1);
			floatY = atof(buffer);
			memset(&buffer[0], 0, sizeof(buffer));
			tempFrame.UVs[BitmapClass::BOTTOM_LEFT] = XMFLOAT2(floatX / imageWidth, floatY / imageHeight);

			// BOTTOM RIGHT
			getline(file, line);
			commaIndex = line.find(",");
			endIndex = line.find("\r");
			line.copy(buffer, commaIndex, 0);
			floatX = atof(buffer);
			line.copy(buffer, endIndex - commaIndex, commaIndex + 1);
			floatY = atof(buffer);
			memset(&buffer[0], 0, sizeof(buffer));
			tempFrame.UVs[BitmapClass::BOTTOM_RIGHT] = XMFLOAT2(floatX / imageWidth, floatY / imageHeight);

			m_FrameList.push_back(tempFrame);
		}
	}
}