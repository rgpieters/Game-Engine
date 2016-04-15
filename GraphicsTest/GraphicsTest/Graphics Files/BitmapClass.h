#ifndef _BITMAP_CLASS_H_
#define _BITMAP_CLASS_H_

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

class BitmapClass
{
public:
	enum TEXT_CORD { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };

	BitmapClass();
	BitmapClass(const BitmapClass& other);
	~BitmapClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename, const char* basisShaderFileName, XMFLOAT2 uvs[4],
						float bitmapWidth, float bitmapHeight, RECT* collisionRect = NULL);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
	bool InitializeTilemapBitmap(const BitmapClass& other, ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMFLOAT2 uvs[4], float tileWidth, float tileHeight);
	

	XMMATRIX GetWorldMatrix();
	void SetPosition(XMFLOAT2& position);
	void SetRotation(float rotation);
	void SetScale(XMFLOAT2& scale);

	void SetRenderCollisionRect(bool render) { m_renderCollisionRect = render; }
	void SetUVs(XMFLOAT2 uvs[4]);	
	void FlipBitmap();
private:
	struct VertexPos
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex0;
	};

	struct RectVertexPos
	{
		XMFLOAT3 pos;
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

	XMFLOAT2 m_UVs[4];
	XMFLOAT2 m_prevousFirstUV;
	XMFLOAT2 m_position;
	XMFLOAT2 m_scale;
	float m_bitmapWidth;
	float m_bitmapHeight;
	float m_rotation;
	bool m_renderCollisionRect;
	bool m_InitialRender;

	// Bitmap Rendering
	ID3D11ShaderResourceView* m_textureView;
	ID3D11VertexShader* m_textureVS;
	ID3D11PixelShader* m_texturePS;
	ID3D11InputLayout* m_inputLayout;
	ID3D11Texture2D* m_texture;
	ID3D11SamplerState* m_samplerState;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	ID3D11Buffer* m_constantBuffer;
	ID3D11BlendState* m_alphaBlendState;

	// Collision Rect Rendering
	ID3D11VertexShader* m_collisionRectVS;
	ID3D11PixelShader* m_collisionRectPS;
	ID3D11InputLayout* m_collisionRectInputLayout;
	ID3D11Buffer* m_collisionRectVertexBuffer;

	XMMATRIX m_worldMatrix;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectiondMatrix;

	void UpdateBuffers(ID3D11DeviceContext* deviceContext);
	bool InitializeShaders(ID3D11Device* device, const char* basisShaderFileName);
	bool InitializeTextureView(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename);
	bool InitializeBuffers(ID3D11Device* device, XMFLOAT2 uvs[4], float bitmapWidth, float bitmapHeight);
	bool InitializeBlending(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	bool InitializeCollisionRectVariables(ID3D11Device* device, ID3D11DeviceContext* deviceContext, RECT collisionRect);
	
	bool CompileD3DShader(char* shaderFileName, char* shaderEntryPoint, char* shaderModelVersion, ID3DBlob** buffer);
	bool LoadTarga(char* filename, int& height, int& width);
	bool UpdateVertexBuffer(ID3D11DeviceContext* deviceContext, XMFLOAT2 uvs[4]);
};

#endif