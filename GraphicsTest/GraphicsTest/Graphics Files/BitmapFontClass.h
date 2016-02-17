#ifndef _BITMAP_FONT_CLASS
#define _BITMAP_FONT_CLASS

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "WICTextureLoader.h"// thanks to Chuck Walbourn, GitHub

#include<d3d11.h>
#include <d3dcompiler.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <directxmath.h>
using namespace DirectX;
using namespace std;

class BitmapFontClass
{
public:
	BitmapFontClass();
	BitmapFontClass(const BitmapFontClass& other);
	~BitmapFontClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename, const char* basisShaderFileName);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, char* message, float startX, float startY, XMFLOAT4 pixelColor);

	XMMATRIX GetWorldMatrix();
	void SetPosition(XMFLOAT2& position);

private:
	struct VertexPos
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex0;
	};
	struct PixelBufferType
	{
		XMFLOAT4 pixelColor;
	};

	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
	unsigned char* m_targaData;

	XMFLOAT2 m_position;

	// Bitmap Rendering
	ID3D11ShaderResourceView* m_textureView;
	ID3D11VertexShader* m_textureVS;
	ID3D11PixelShader* m_texturePS;
	ID3D11InputLayout* m_inputLayout;
	ID3D11Texture2D* m_texture;
	ID3D11SamplerState* m_samplerState;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_pixelBuffer; // Needs initialization and shutdown
	ID3D11Buffer* m_indexBuffer;
	ID3D11Buffer* m_constantBuffer;
	ID3D11BlendState* m_alphaBlendState;

	XMMATRIX m_worldMatrix;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectiondMatrix;

	bool InitializeShaders(ID3D11Device* device, const char* basisShaderFileName);
	bool InitializeTextureView(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename);
	bool InitializeBuffers(ID3D11Device* device);
	bool InitializeBlending(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	bool CompileD3DShader(char* shaderFileName, char* shaderEntryPoint, char* shaderModelVersion, ID3DBlob** buffer);
	bool LoadTarga(char* filename, int& height, int& width);

	void DrawString(ID3D11DeviceContext* deviceContext, char* message, float startX, float startY, XMFLOAT4 pixelColor);
};

#endif