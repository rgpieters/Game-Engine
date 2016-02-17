#include "BitmapClass.h"

BitmapClass::BitmapClass() : m_rotation(0)
{
	m_scale.x = m_scale.y = 1.0f;
	m_targaData = NULL;
	m_texture = NULL;

	m_textureView = NULL;
	m_textureVS = NULL;
	m_texturePS = NULL;
	m_inputLayout = NULL;
	m_texture = NULL;
	m_samplerState = NULL;
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_constantBuffer = NULL;
	m_alphaBlendState = NULL;

	m_collisionRectVS = NULL;
	m_collisionRectPS = NULL;
	m_collisionRectInputLayout = NULL;
	m_collisionRectVertexBuffer = NULL;

	m_renderCollisionRect = false;
}

BitmapClass::BitmapClass(const BitmapClass& other)
{
}

BitmapClass::~BitmapClass()
{
}

bool BitmapClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename, const char* basisShaderFileName, XMFLOAT2 uvs[4],
								float bitmapWidth, float bitmapHeight, RECT* collisionRect)
{	
	bool initResult = InitializeShaders(device, basisShaderFileName);
	if (!initResult)
	{
		return false;
	}

	initResult = InitializeTextureView(device, deviceContext, textureFilename);
	if (!initResult)
	{
		return false;
	}

	initResult = InitializeBuffers(device, uvs, bitmapWidth, bitmapHeight);
	if (!initResult)
	{
		return false;
	}

	initResult = InitializeBlending(device, deviceContext);
	if (!initResult)
	{
		return false;
	}

	if (collisionRect != NULL)
	{
		InitializeCollisionRectVariables(device, deviceContext, *collisionRect);
	}
	
	return true;
}

void BitmapClass::Shutdown()
{
	if (m_targaData)
	{
		delete[] m_targaData;
		m_targaData = NULL;
	}
	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = NULL;
	}
	if (m_textureVS)
	{
		m_textureVS->Release();
		m_textureVS = NULL;
	}
	if (m_texturePS)
	{
		m_texturePS->Release();
		m_texturePS = NULL;
	}
	if (m_inputLayout)
	{
		m_inputLayout->Release();
		m_inputLayout = NULL;
	}
	if (m_texture)
	{
		m_texture->Release();
		m_texture = NULL;
	}
	if (m_samplerState)
	{
		m_samplerState->Release();
		m_samplerState = NULL;
	}
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = NULL;
	}
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = NULL;
	}
	if (m_constantBuffer)
	{
		m_constantBuffer->Release();
		m_constantBuffer = NULL;
	}
	if (m_alphaBlendState)
	{
		m_alphaBlendState->Release();
		m_alphaBlendState = NULL;
	}
	if (m_collisionRectVS)
	{
		m_collisionRectVS->Release();
		m_collisionRectVS = NULL;
	}
	if (m_collisionRectPS)
	{
		m_collisionRectPS->Release();
		m_collisionRectPS = NULL;
	}
	if (m_collisionRectInputLayout)
	{
		m_collisionRectInputLayout->Release();
		m_collisionRectInputLayout = NULL;
	}
	if (m_collisionRectVertexBuffer)
	{
		m_collisionRectVertexBuffer->Release();
		m_collisionRectVertexBuffer = NULL;
	}

}

bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	if (!m_renderCollisionRect)
	{
		unsigned int stride = sizeof(VertexPos);
		unsigned int offset = 0;

		UpdateBuffers(deviceContext);

		deviceContext->IASetInputLayout(m_inputLayout);
		deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		deviceContext->VSSetShader(m_textureVS, 0, 0);
		deviceContext->PSSetShader(m_texturePS, 0, 0);
		deviceContext->PSSetShaderResources(0, 1, &m_textureView);
		deviceContext->PSSetSamplers(0, 1, &m_samplerState);

		XMMATRIX viewProjectionMatrix = DirectX::XMMatrixMultiply(viewMatrix, projectionMatrix);
		XMMATRIX modelViewProjectionMatrix = DirectX::XMMatrixMultiply(GetWorldMatrix(), viewProjectionMatrix);
		modelViewProjectionMatrix = DirectX::XMMatrixTranspose(modelViewProjectionMatrix);

		deviceContext->UpdateSubresource(m_constantBuffer, 0, 0, &modelViewProjectionMatrix, 0, 0);
		deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);

		deviceContext->DrawIndexed(6, 0, 0);

		return true;
	}
	else
	{
		unsigned int stride = sizeof(VertexPos);
		unsigned int offset = 0;

		deviceContext->IASetInputLayout(m_collisionRectInputLayout);
		deviceContext->IASetVertexBuffers(0, 1, &m_collisionRectVertexBuffer, &stride, &offset);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		deviceContext->VSSetShader(m_collisionRectVS, 0, 0);
		deviceContext->PSSetShader(m_collisionRectPS, 0, 0);

		XMMATRIX viewProjectionMatrix = DirectX::XMMatrixMultiply(viewMatrix, projectionMatrix);
		XMMATRIX modelViewProjectionMatrix = DirectX::XMMatrixMultiply(GetWorldMatrix(), viewProjectionMatrix);
		modelViewProjectionMatrix = DirectX::XMMatrixTranspose(modelViewProjectionMatrix);

		deviceContext->UpdateSubresource(m_constantBuffer, 0, 0, &modelViewProjectionMatrix, 0, 0);
		deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);

		deviceContext->Draw(6, 0);

		return true;
	}
}

void BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexPos* verticesPtr;
	HRESULT result;

	if (m_prevousFirstUV.x == m_UVs[0].x && m_prevousFirstUV.y == m_UVs[0].y)
		return;

	VertexPos vertices[6] =
	{
		{ XMFLOAT3(m_bitmapWidth / 2.0f, m_bitmapHeight / 2.0f, 1.0f), m_UVs[TOP_RIGHT] },
		{ XMFLOAT3(m_bitmapWidth / 2.0f, -m_bitmapHeight / 2.0f, 1.0f), m_UVs[BOTTOM_RIGHT] },
		{ XMFLOAT3(-m_bitmapWidth / 2.0f, -m_bitmapHeight / 2.0f, 1.0f), m_UVs[BOTTOM_LEFT] },

		{ XMFLOAT3(-m_bitmapWidth / 2.0f, -m_bitmapHeight / 2.0f, 1.0f), m_UVs[BOTTOM_LEFT] },
		{ XMFLOAT3(-m_bitmapWidth / 2.0f, m_bitmapHeight / 2.0f, 1.0f), m_UVs[TOP_LEFT] },
		{ XMFLOAT3(m_bitmapWidth / 2.0f, m_bitmapHeight / 2.0f, 1.0f), m_UVs[TOP_RIGHT] },
	};

	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		MessageBox(0, L"Error locking vertex buffer!", L"Updating Buffer Error", MB_OK);
	}

	verticesPtr = (VertexPos*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexPos) * 6));

	deviceContext->Unmap(m_vertexBuffer, 0);
}

bool BitmapClass::InitializeShaders(ID3D11Device* device, const char* basisShaderFileName)
{
	char* vertexShaderFileName = (char*)malloc(1 + strlen(basisShaderFileName) + strlen(".vs"));
	char* pixelShaderFileName = (char*)malloc(1 + strlen(basisShaderFileName) + strlen(".ps"));

	strcpy_s(vertexShaderFileName, 1 + strlen(basisShaderFileName), basisShaderFileName);
	strcat_s(vertexShaderFileName, 1 + strlen(basisShaderFileName) + strlen(".vs"), ".vs");

	strcpy_s(pixelShaderFileName, 1 + strlen(basisShaderFileName), basisShaderFileName);
	strcat_s(pixelShaderFileName, 1 + strlen(basisShaderFileName) + strlen(".ps"), ".ps");

	// Vertex Shader
	ID3DBlob* vertexShaderBuffer = NULL;

	bool compileResult = CompileD3DShader(vertexShaderFileName, "VS_Main", "vs_4_0", &vertexShaderBuffer);

	if (!compileResult)
	{
		if (vertexShaderFileName)
			free(vertexShaderFileName);
		if (pixelShaderFileName)
			free(pixelShaderFileName);
		MessageBox(0, L"Error loading vertex shader!", L"Compile Error", MB_OK);
		return false;
	}

	HRESULT result;

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_textureVS);
	if (FAILED(result))
	{
		if (vertexShaderBuffer)
			vertexShaderBuffer->Release();
		if (vertexShaderFileName)
			free(vertexShaderFileName);
		if (pixelShaderFileName)
			free(pixelShaderFileName);
		MessageBox(0, L"Error creating vertex shader!", L"Compile Error", MB_OK);
		return false;
	}

	// Pixel Shader
	ID3DBlob* pixelShaderBuffer = NULL;

	compileResult = CompileD3DShader(pixelShaderFileName, "PS_Main", "ps_4_0", &pixelShaderBuffer);
	if (!compileResult)
	{
		if (vertexShaderFileName)
			free(vertexShaderFileName);
		if (pixelShaderFileName)
			free(pixelShaderFileName);
		MessageBox(0, L"Error loading pixel shader!", L"Compile Error", MB_OK);
		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_texturePS);
	if (FAILED(result))
	{
		if (pixelShaderBuffer)
			pixelShaderBuffer->Release();
		if (vertexShaderFileName)
			free(vertexShaderFileName);
		if (pixelShaderFileName)
			free(pixelShaderFileName);
		MessageBox(0, L"Error creating pixel shader!", L"Compile Error", MB_OK);
		return false;
	}

	free(vertexShaderFileName);
	free(pixelShaderFileName);

	// Input Layer
	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

	result = device->CreateInputLayout(solidColorLayout, totalLayoutElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_inputLayout);
	if (FAILED(result))
	{
		if (pixelShaderBuffer)
			pixelShaderBuffer->Release();
		if (vertexShaderFileName)
			free(vertexShaderFileName);
		if (pixelShaderFileName)
			free(pixelShaderFileName);
		MessageBox(0, L"Error creating input layout!", L"Compile Error", MB_OK);
		return false;
	}

	return true;
}

bool BitmapClass::InitializeTextureView(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename)
{
	HRESULT result;

	// Texture View
	D3D11_TEXTURE2D_DESC texture2DDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc;

	int fileNameLength = strlen(textureFilename);
	string fileExtenstion;
	string strFileName(textureFilename);

	for (int i = fileNameLength - 3; i < fileNameLength; i++)
	{
		fileExtenstion += strFileName.at(i);
	}

	if (fileExtenstion != "tga")
	{
		HRESULT hResult;
		wostringstream ss;
		ss << textureFilename;

		std::basic_ifstream<unsigned char> file(textureFilename, std::ios::binary);

		if (file.is_open())
		{
			file.seekg(0, ios::end);
			int length = (int)file.tellg();
			file.seekg(0, ios::beg);

			unsigned char* buffer = new unsigned char[length];
			file.read(&buffer[0], length);
			file.close();

			hResult = CreateWICTextureFromMemory(device, deviceContext, &buffer[0], (size_t)length, NULL, &m_textureView, NULL);
			if (FAILED(hResult))
			{
				MessageBox(0, L"Failed to create texture from memory!", L"Compile Error", MB_OK);
				return false;
			}
		}
		else
		{
			MessageBox(0, L"Failed to open texture file!", L"Compile Error", MB_OK);
			return false;
		}
	}
	else
	{
		int height, width;
		unsigned int rowPitch;

		// Load the targa image data into memory.
		result = LoadTarga(textureFilename, height, width);
		if (FAILED(result))
		{
			if (m_targaData)
			{
				delete[] m_targaData;
				m_targaData = NULL;
			}
			MessageBox(0, L"Failed to load Targa image data into memory!", L"Compile Error", MB_OK);
			return false;
		}

		// Setup the description of the texture.
		texture2DDesc.Height = height;
		texture2DDesc.Width = width;
		texture2DDesc.MipLevels = 0;
		texture2DDesc.ArraySize = 1;
		texture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture2DDesc.SampleDesc.Count = 1;
		texture2DDesc.SampleDesc.Quality = 0;
		texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
		texture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texture2DDesc.CPUAccessFlags = 0;
		texture2DDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		// Create the empty texture.
		result = device->CreateTexture2D(&texture2DDesc, NULL, &m_texture);
		if (FAILED(result))
		{
			delete[] m_targaData;
			m_targaData = NULL;
			MessageBox(0, L"Failed to create empty Targa texture!", L"Compile Error", MB_OK);
			return false;
		}

		// Set the row pitch of the targa image data.
		rowPitch = (width * 4) * sizeof(unsigned char);

		// Copy the targa image data into the texture.
		deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

		// Setup the shader resource view description.
		shaderDesc.Format = texture2DDesc.Format;
		shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderDesc.Texture2D.MostDetailedMip = 0;
		shaderDesc.Texture2D.MipLevels = -1;

		// Create the shader resource view for the texture.
		result = device->CreateShaderResourceView(m_texture, &shaderDesc, &m_textureView);
		if (FAILED(result))
		{
			delete[] m_targaData;
			m_targaData = NULL;
			MessageBox(0, L"Failed to create shader resource view for Targa texture", L"Compile Error", MB_OK);
			return false;
		}
	}
	deviceContext->GenerateMips(m_textureView);

	if (m_targaData)
	{
		delete[] m_targaData;
		m_targaData = NULL;
	}

	// Sample State
	D3D11_SAMPLER_DESC sampleStateDesc;
	ZeroMemory(&sampleStateDesc, sizeof(sampleStateDesc));
	sampleStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP; //D3D11_TEXTURE_ADDRESS_WRAP
	sampleStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleStateDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampleStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&sampleStateDesc, &m_samplerState);
	if (FAILED(result))
	{
		MessageBox(0, L"Failed to create sampler state!", L"Compile Error", MB_OK);
		return false;
	}

	return true;
}

bool BitmapClass::InitializeBuffers(ID3D11Device* device, XMFLOAT2 uvs[4], float bitmapWidth, float bitmapHeight)
{
	HRESULT result;

	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// Vertex Buffer
	D3D11_TEXTURE2D_DESC textureDesc;

	if (m_texture)
	{
		m_texture->GetDesc(&textureDesc);
	}
	else
	{
		ID3D11Resource* texture;
		m_textureView->GetResource(&texture);
		((ID3D11Texture2D*)texture)->GetDesc(&textureDesc);
		texture->Release();
	}

	for (int i = 0; i < 4; i++)
	{
		m_UVs[i].x = uvs[i].x;
		m_UVs[i].y = uvs[i].y;
	}

	m_prevousFirstUV.x = m_UVs[0].x;
	m_prevousFirstUV.y = m_UVs[0].y;

	VertexPos vertices[6] =
	{
		{ XMFLOAT3(bitmapWidth / 2.0f, bitmapHeight / 2.0f, 1.0f), uvs[TOP_RIGHT] },
		{ XMFLOAT3(bitmapWidth / 2.0f, -bitmapHeight / 2.0f, 1.0f), uvs[BOTTOM_RIGHT] },
		{ XMFLOAT3(-bitmapWidth / 2.0f, -bitmapHeight / 2.0f, 1.0f), uvs[BOTTOM_LEFT] },

		{ XMFLOAT3(-bitmapWidth / 2.0f, -bitmapHeight / 2.0f, 1.0f), uvs[BOTTOM_LEFT] },
		{ XMFLOAT3(-bitmapWidth / 2.0f, bitmapHeight / 2.0f, 1.0f), uvs[TOP_LEFT] },
		{ XMFLOAT3(bitmapWidth / 2.0f, bitmapHeight / 2.0f, 1.0f), uvs[TOP_RIGHT] },
	};

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 6;
	vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;

	result = device->CreateBuffer(&vertexDesc, &resourceData, &m_vertexBuffer);
	if (FAILED(result))
	{
		MessageBox(0, L"Failed to create vertex buffer!", L"Compile Error", MB_OK);
		return false;
	}

	// Index Buffer
	unsigned long* indices;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	int indexCount = ARRAYSIZE(vertices);
	indices = new unsigned long[indexCount];
	if (!indices)
	{
		MessageBox(0, L"Failed to create index array", L"Compile Error", MB_OK);
		return false;
	}

	for (int i = 0; i< indexCount; i++)
	{
		indices[i] = i;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		MessageBox(0, L"Failed to create index buffer", L"Compile Error", MB_OK);
		return false;
	}

	D3D11_BUFFER_DESC constDesc;
	ZeroMemory(&constDesc, sizeof(constDesc));
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof(XMMATRIX);
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	result = device->CreateBuffer(&constDesc, 0, &m_constantBuffer);
	if (FAILED(result))
	{
		delete[] indices;
		indices = NULL;
		MessageBox(0, L"Failed to create vertex buffer!", L"Compile Error", MB_OK);
		return false;
	}

	delete[] indices;
	indices = NULL;

	return true;
}

bool BitmapClass::InitializeBlending(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// Blend
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	device->CreateBlendState(&blendDesc, &m_alphaBlendState);
	deviceContext->OMSetBlendState(m_alphaBlendState, blendFactor, 0xFFFFFFFF);

	// Rasterizer State Object
	ID3D11RasterizerState * RasterState;
	D3D11_RASTERIZER_DESC rasterizerState;

	rasterizerState.FillMode = D3D11_FILL_SOLID;
	rasterizerState.CullMode = D3D11_CULL_NONE;
	rasterizerState.FrontCounterClockwise = true;
	rasterizerState.DepthBias = false;
	rasterizerState.DepthBiasClamp = 0;
	rasterizerState.SlopeScaledDepthBias = 0;
	rasterizerState.DepthClipEnable = true;
	rasterizerState.ScissorEnable = true;
	rasterizerState.MultisampleEnable = false;
	rasterizerState.AntialiasedLineEnable = false;
	
	device->CreateRasterizerState(&rasterizerState, &RasterState);
	deviceContext->RSSetState(RasterState);

	return true;
}

bool BitmapClass::InitializeTilemapBitmap(const BitmapClass& other, ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMFLOAT2 uvs[4], float tileWidth, float tileHeight)
{
	for (int i = 0; i < 4; i++)
	{
		m_UVs[i] = uvs[i];
	}

	m_position = other.m_position;
	m_scale = other.m_scale;
	m_rotation = other.m_rotation;

	m_textureView = other.m_textureView;
	m_textureVS = other.m_textureVS;
	m_texturePS = other.m_texturePS;
	m_inputLayout = other.m_inputLayout;
	m_texture = other.m_texture;
	m_samplerState = other.m_samplerState;
	m_constantBuffer = other.m_constantBuffer;
	m_alphaBlendState = other.m_alphaBlendState;
	m_vertexBuffer = other.m_vertexBuffer;
	m_worldMatrix = other.m_worldMatrix;
	m_viewMatrix = other.m_viewMatrix;
	m_projectiondMatrix = other.m_projectiondMatrix;

	InitializeBuffers(device, uvs, tileWidth, tileHeight);

	return true;
}

bool BitmapClass::InitializeCollisionRectVariables(ID3D11Device* device, ID3D11DeviceContext* deviceContext, RECT collisionRect)
{
	ID3DBlob* vsBuffer = NULL;	
	ID3DBlob* psBuffer = NULL;
	HRESULT result;


	bool compileResult = CompileD3DShader("Resource Files/collisionRectShader.vs", "VS_Main", "vs_4_0", &vsBuffer);
	if (compileResult == false)
	{
		MessageBox(0, L"Error loading vertex shader!", L"Compile Error", MB_OK);
		return false;
	}

	result = device->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, &m_collisionRectVS);
	if (FAILED(result))
	{
		if (vsBuffer)
			vsBuffer->Release();

		MessageBox(0, L"Failed to create collision rect vertex shader!", L"Compile Error", MB_OK);
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

	result = device->CreateInputLayout(solidColorLayout, totalLayoutElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &m_collisionRectInputLayout);

	vsBuffer->Release();

	if (FAILED(result))
	{
		MessageBox(0, L"Failed to create collision rect input layout!", L"Compile Error", MB_OK);
		return false;
	}

	compileResult = CompileD3DShader("Resource Files/collisionRectShader.ps", "PS_Main", "ps_4_0", &psBuffer);

	if (compileResult == false)
	{
		MessageBox(0, L"Error loading pixel shader!", L"Compile Error", MB_OK);
		return false;
	}

	result = device->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), 0, &m_collisionRectPS);

	psBuffer->Release();

	if (FAILED(result))
	{
		MessageBox(0, L"Failed to create collision rect pixel shader!", L"Compile Error", MB_OK);
		return false;
	}

	//RectVertexPos vertices[] =
	//{
	//	XMFLOAT3(collisionRect.left, collisionRect.bottom, 1.0f),
	//	XMFLOAT3(collisionRect.left, collisionRect.top, 1.0f),
	//	XMFLOAT3(collisionRect.right, collisionRect.top, 1.0f),

	//	XMFLOAT3(collisionRect.right, collisionRect.top, 1.0f),
	//	XMFLOAT3(collisionRect.right, collisionRect.bottom, 1.0f),
	//	XMFLOAT3(collisionRect.left, collisionRect.bottom, 1.0f)
	//};

	RectVertexPos vertices[] =
	{
		XMFLOAT3(0.5f, 0.5f, 1.0f),
		XMFLOAT3(0.5f, -0.5f, 1.0f),
		XMFLOAT3(-0.5f, -0.5f, 1.0f)
	};

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 3;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;

	result = device->CreateBuffer(&vertexDesc, &resourceData, &m_collisionRectVertexBuffer);

	if (FAILED(result))
	{
		MessageBox(0, L"Failed to create collision rect vertex buffer!", L"Compile Error", MB_OK);
		return false;
	}

	return true;
}

bool BitmapClass::CompileD3DShader(char* shaderFileName, char* shaderEntryPoint, char* shaderModelVersion, ID3DBlob** buffer)
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3DBlob* errorBuffer = 0;
	HRESULT result;
	WCHAR* tempFilePath;

	int nCharsVertex = MultiByteToWideChar(CP_ACP, 0, shaderFileName, -1, NULL, 0);
	tempFilePath = new WCHAR[nCharsVertex];
	MultiByteToWideChar(CP_ACP, 0, shaderFileName, -1, tempFilePath, nCharsVertex);

	result = D3DCompileFromFile(tempFilePath, NULL, NULL, shaderEntryPoint, shaderModelVersion, shaderFlags, NULL, buffer, &errorBuffer);
	if (FAILED(result))
	{
		if (errorBuffer != 0)
		{
			OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}

		return false;
	}

	if (errorBuffer != 0)
		errorBuffer->Release();

	return true;
}

bool BitmapClass::LoadTarga(char* filename, int& height, int& width)
{
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Get the important information from the header.
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if (bpp != 32)
	{
		return false;
	}

	// Calculate the size of the 32 bit image data.
	imageSize = width * height * 4;

	// Allocate memory for the targa image data.
	targaImage = new unsigned char[imageSize];
	if (!targaImage)
	{
		return false;
	}

	// Read in the targa image data.
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		delete[] targaImage;
		targaImage = NULL;
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		delete[] targaImage;
		targaImage = NULL;
		return false;
	}

	// Allocate memory for the targa destination data.
	m_targaData = new unsigned char[imageSize];
	if (!m_targaData)
	{
		delete[] targaImage;
		targaImage = NULL;
		return false;
	}

	// Initialize the index into the targa destination data array.
	index = 0;

	// Initialize the index into the targa image data.
	k = (width * height * 4) - (width * 4);

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
	for (j = 0; j<height; j++)
	{
		for (i = 0; i<width; i++)
		{
			m_targaData[index + 0] = targaImage[k + 2];  // Red.
			m_targaData[index + 1] = targaImage[k + 1];  // Green.
			m_targaData[index + 2] = targaImage[k + 0];  // Blue
			m_targaData[index + 3] = targaImage[k + 3];  // Alpha

			// Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (width * 8);
	}

	// Release the targa image data now that it was copied into the destination array.
	delete[] targaImage;
	targaImage = NULL;

	return true;
}

bool BitmapClass::UpdateVertexBuffer(ID3D11DeviceContext* deviceContext, XMFLOAT2 uvs[4])
{
	VertexPos* verticesPtr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;

	VertexPos vertices[6] =
	{
		{ XMFLOAT3(16, 16, 1.0f), uvs[TOP_RIGHT] },
		{ XMFLOAT3(16, -16, 1.0f), uvs[BOTTOM_RIGHT] },
		{ XMFLOAT3(-16, -16, 1.0f), uvs[BOTTOM_LEFT] },

		{ XMFLOAT3(-16, -16, 1.0f), uvs[BOTTOM_LEFT] },
		{ XMFLOAT3(-16, 16, 1.0f), uvs[TOP_LEFT] },
		{ XMFLOAT3(16, 16, 1.0f), uvs[TOP_RIGHT] },
	};

	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		MessageBox(0, L"Error locking vertex buffer!", L"Compile Error", MB_OK);
		return false;
	}

	verticesPtr = (VertexPos*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexPos) * 6));

	deviceContext->Unmap(m_vertexBuffer, 0);

	return true;
}

XMMATRIX BitmapClass::GetWorldMatrix()
{
	XMMATRIX translation = XMMatrixTranslation(m_position.x * m_scale.x, m_position.y, 0.0f);
	XMMATRIX rotationZ = XMMatrixRotationZ(m_rotation);
	XMMATRIX scale = XMMatrixScaling(m_scale.x, m_scale.y, 1.0f);

	return translation * rotationZ * scale;
}

void BitmapClass::SetPosition(XMFLOAT2& position)
{
	m_position = position;
}

void BitmapClass::SetRotation(float rotation)
{
	m_rotation = rotation;
}

void BitmapClass::SetScale(XMFLOAT2& scale)
{
	m_scale = scale;
}

void BitmapClass::SetUVs(XMFLOAT2 uvs[4])
{
	m_prevousFirstUV.x = m_UVs[0].x;
	m_prevousFirstUV.y = m_UVs[0].y;

	for (int i = 0; i < 4; i++)
	{
		m_UVs[i].x = uvs[i].x;
		m_UVs[i].y = uvs[i].y;
	}
}

void BitmapClass::FlipBitmap()
{
	//float tempFloat1 = 1 - m_UVs[TOP_LEFT].x;
	//float tempFloat2 = 1 - m_UVs[TOP_RIGHT].x;

	//m_UVs[TOP_LEFT].x = 1 - tempFloat1;
	//m_UVs[TOP_RIGHT].x = 1 - tempFloat2;
	//m_UVs[BOTTOM_LEFT].x = 1 - tempFloat1;
	//m_UVs[BOTTOM_RIGHT].x = 1 - tempFloat2;

	//int x = 10;

	//m_prevousFirstUV.x = 1 - m_prevousFirstUV.x;
	//for (int i = 0; i < 4; i++)
	//{
	//	m_UVs[i].x = 1 - m_UVs[i].x;
	//}
}