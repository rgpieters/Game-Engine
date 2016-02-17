#include "BitmapFontClass.h"

BitmapFontClass::BitmapFontClass()
{
	m_targaData = NULL;
	m_texture = NULL;

	m_textureView = NULL;
	m_textureVS = NULL;
	m_texturePS = NULL;
	m_inputLayout = NULL;
	m_texture = NULL;
	m_samplerState = NULL;
	m_vertexBuffer = NULL;
	m_pixelBuffer = NULL;
	m_indexBuffer = NULL;
	m_constantBuffer = NULL;
	m_alphaBlendState = NULL;
}

BitmapFontClass::BitmapFontClass(const BitmapFontClass& other)
{
}

BitmapFontClass::~BitmapFontClass()
{
}

bool BitmapFontClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename, const char* basisShaderFileName)
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

	initResult = InitializeBuffers(device);
	if (!initResult)
	{
		return false;
	}

	initResult = InitializeBlending(device, deviceContext);
	if (!initResult)
	{
		return false;
	}

	return true;
}

void BitmapFontClass::Shutdown()
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
	if (m_pixelBuffer)
	{
		m_pixelBuffer->Release();
		m_pixelBuffer = NULL;
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
}

bool BitmapFontClass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, char* message, float startX, float startY, XMFLOAT4 pixelColor)
{
	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;

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

	DrawString(deviceContext, message, startX, startY, pixelColor);
	//DrawString(deviceContext, "Hello World", startX, startY, pixelColor);

	return true;
}

void BitmapFontClass::DrawString(ID3D11DeviceContext* deviceContext, char* message, float startX, float startY, XMFLOAT4 pixelColor)
{
	const int sizeOfSprite = sizeof(VertexPos) * 6;
	const int maxLetters = 24;
	int length = strlen(message);

	if (length > maxLetters)
	{
		length = maxLetters;
	}

	float charWidth = 32.0f;
	float charHeight = 32.0f;

	const int verticesPerLetter = 6;

	D3D11_MAPPED_SUBRESOURCE mapResource;
	HRESULT d3dResult = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	if (FAILED(d3dResult))
	{
		MessageBox(0, L"Failed to map resource!", L"BitmapFont Error", MB_OK);
		return;
	}

	VertexPos* spritePtr = (VertexPos*)mapResource.pData;
	const int indexA = static_cast<char>('A');
	const int indexZ = static_cast<char>('Z');

	for (int i = 0; i < length; i++)
	{
		float thisStartX = startX + (charWidth * static_cast<float>(i));
		float thisEndX = thisStartX + charWidth;
		float thisEndY = startY + charHeight;

		spritePtr[0].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);
		spritePtr[1].pos = XMFLOAT3(thisEndX, startY, 1.0f);
		spritePtr[2].pos = XMFLOAT3(thisStartX, startY, 1.0f);
		spritePtr[3].pos = XMFLOAT3(thisStartX, startY, 1.0f);
		spritePtr[4].pos = XMFLOAT3(thisStartX, thisEndY, 1.0f);
		spritePtr[5].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);

		int texLookup = 0;
		int letter = static_cast<char>(message[i]);

		if (letter < indexA || letter > indexZ)
		{
			// Grab one index past Z, which is a blank space in the texture.
			texLookup = (indexZ - indexA) + 1;
		}
		else
		{
			// A = 0, B = 1, Z = 25, etc.
			texLookup = (letter - indexA);
		}
		
		int zeroBasedIndex = letter;// -32;
		int tilesAmountX = 16;
		int tilesAmountY = 16;
		int indexX = zeroBasedIndex % tilesAmountX;
		int indexY = zeroBasedIndex / tilesAmountX;

		float startX = (float)indexX / tilesAmountX;
		float endX = (float)(indexX + 1.0f) / tilesAmountX;
		float startY = (float)indexY / tilesAmountY;
		float endY = (float)(indexY + 1.0f) / tilesAmountY;

		spritePtr[0].tex0 = XMFLOAT2(endX, startY);		// TOP RIGHT
		spritePtr[1].tex0 = XMFLOAT2(endX, endY);		// BOTTOM RIGHT
		spritePtr[2].tex0 = XMFLOAT2(startX, endY);	// BOTTOM LEFT
		spritePtr[3].tex0 = XMFLOAT2(startX, endY);	// BOTTOM LEFT
		spritePtr[4].tex0 = XMFLOAT2(startX, startY);	// TOP LEFT
		spritePtr[5].tex0 = XMFLOAT2(endX, startY);		// TOP RIGHT

		spritePtr += 6;
	}

	deviceContext->Unmap(m_vertexBuffer, 0);

	deviceContext->PSSetShaderResources(0, 1, &m_textureView);
	unsigned int bufferNumber;
	PixelBufferType* dataPtr2;

	d3dResult = deviceContext->Map(m_pixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	if (FAILED(d3dResult))
	{
		MessageBox(0, L"Failed to map resource!", L"BitmapFont Error", MB_OK);
		return;
	}

	dataPtr2 = (PixelBufferType*)mapResource.pData;
	dataPtr2->pixelColor = pixelColor;

	deviceContext->Unmap(m_pixelBuffer, 0);

	bufferNumber = 0;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pixelBuffer);


	deviceContext->Draw(6 * length, 0);
}

bool BitmapFontClass::InitializeShaders(ID3D11Device* device, const char* basisShaderFileName)
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

	compileResult = CompileD3DShader(/*pixelShaderFileName*/"Resource Files/myColorFont.ps", "PS_Main", "ps_4_0", &pixelShaderBuffer);
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

bool BitmapFontClass::InitializeTextureView(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename)
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

	if (fileExtenstion != "tga" && fileExtenstion != "dds")
	{
		HRESULT hResult;
		textureFilename = "Resource Files/font_bitmap.png";
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
	else if (fileExtenstion == "dds")
	{
		HRESULT hResult;
		WCHAR* tempFilePath;
		int nCharsVertex = MultiByteToWideChar(CP_ACP, 0, textureFilename, -1, NULL, 0);
		tempFilePath = new WCHAR[nCharsVertex];
		MultiByteToWideChar(CP_ACP, 0, textureFilename, -1, tempFilePath, nCharsVertex);

		hResult = CreateWICTextureFromFile(device, tempFilePath, NULL, &m_textureView, NULL);
		if (FAILED(hResult))
		{
			MessageBox(0, L"Failed to load the texture image!", L"Compile Error", MB_OK);
			delete tempFilePath;
			return false;
		}
		delete tempFilePath;
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
	sampleStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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

bool BitmapFontClass::InitializeBuffers(ID3D11Device* device)
{
	HRESULT result;

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

	const int sizeOfSprite = sizeof(VertexPos) * 6;
	const int maxLetters = 24;

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeOfSprite * maxLetters;

	result = device->CreateBuffer(&vertexDesc, NULL, &m_vertexBuffer);
	if (FAILED(result))
	{
		MessageBox(0, L"Failed to create vertex buffer!", L"Compile Error", MB_OK);
		return false;
	}

	// Index Buffer
	unsigned long* indices;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	int indexCount = 6;
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

	D3D11_BUFFER_DESC pixelBufferDesc;
	pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
	pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pixelBufferDesc.MiscFlags = 0;
	pixelBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&pixelBufferDesc, NULL, &m_pixelBuffer);
	if (FAILED(result))
	{
		MessageBox(0, L"Failed to create pixel buffer!", L"Compile Error", MB_OK);
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
		MessageBox(0, L"Failed to create constant buffer!", L"Compile Error", MB_OK);
		return false;
	}

	delete[] indices;
	indices = NULL;

	return true;
}

bool BitmapFontClass::InitializeBlending(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
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

	return true;
}

bool BitmapFontClass::CompileD3DShader(char* shaderFileName, char* shaderEntryPoint, char* shaderModelVersion, ID3DBlob** buffer)
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

bool BitmapFontClass::LoadTarga(char* filename, int& height, int& width)
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

XMMATRIX BitmapFontClass::GetWorldMatrix()
{
	XMMATRIX translation = XMMatrixTranslation(m_position.x, m_position.y, 0.0f);
	XMMATRIX rotationZ = XMMatrixRotationZ(0.0f);
	XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	return translation * rotationZ * scale;
}

void BitmapFontClass::SetPosition(XMFLOAT2& position)
{
	m_position = position;
}