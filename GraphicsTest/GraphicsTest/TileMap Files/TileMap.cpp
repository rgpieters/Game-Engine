#include "TileMap.h"

TileMap::TileMap()
{
}

TileMap::TileMap(const TileMap& other)
{
}

TileMap::~TileMap()
{
}

void TileMap::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int posX, int posY)
{
	LoadTMXFile("Resource Files/tileset/example.tmx");
	InitializeTilemapBitmaps(device, deviceContext);

	XMFLOAT2 uvs[4];
	uvs[BitmapClass::TOP_LEFT].x = 0.0;
	uvs[BitmapClass::TOP_LEFT].y = 0.0;
	uvs[BitmapClass::TOP_RIGHT].x = 1.0;
	uvs[BitmapClass::TOP_RIGHT].y = 0.0;
	uvs[BitmapClass::BOTTOM_LEFT].x = 0.0;
	uvs[BitmapClass::BOTTOM_LEFT].y = 1.0;
	uvs[BitmapClass::BOTTOM_RIGHT].x = 1.0;
	uvs[BitmapClass::BOTTOM_RIGHT].y = 1.0;

	for (unsigned int i = 0; i < m_ObjectGroupList.size(); i++)
	{
		BaseObject* tempObject = new BaseObject();
		tempObject->Initialize(device, deviceContext, hwnd, (float)m_ObjectGroupList[i].objectX, -(float)m_ObjectGroupList[i].objectY, "Resource Files/download.png", "Resource Files/myColor", uvs,
								(float)m_ObjectGroupList[i].objectWidth, (float)m_ObjectGroupList[i].objectHeight, BaseObject::COLLISION_OBJECT);
		m_ObjectList.push_back(tempObject);
	}
}

void TileMap::LoadTMXFile(char* fileName)
{
	tinyxml2::XMLDocument tmxFile;
	tmxFile.LoadFile(fileName);
	
	tinyxml2::XMLElement* rootElement = tmxFile.FirstChildElement();
	m_version = strtof(rootElement->Attribute("version"), NULL);
	m_tileMapWidth = atoi(rootElement->Attribute("width"));
	m_tileMapHeight = atoi(rootElement->Attribute("height"));
	m_tileWidth = atoi(rootElement->Attribute("tilewidth"));
	m_tileHeight = atoi(rootElement->Attribute("tileheight"));

	tinyxml2::XMLElement* xmlElement = rootElement->FirstChildElement("tileset");

	while (xmlElement)
	{
		TileSet tempTileSet;

		tempTileSet.firstgid = atoi(xmlElement->Attribute("firstgid"));
		string tempStr = xmlElement->Attribute("name");
		tempTileSet.tileSetName = xmlElement->Attribute("name");
		tempTileSet.tileWidth = atoi(xmlElement->Attribute("tilewidth"));
		tempTileSet.tileHeight = atoi(xmlElement->Attribute("tileheight"));
		
		tinyxml2::XMLElement* xmlElementChild = xmlElement->FirstChildElement("image");
		tempTileSet.imageSource = xmlElementChild->Attribute("source");
		tempTileSet.imageWidth = atoi(xmlElementChild->Attribute("width"));
		tempTileSet.imageHeight = atoi(xmlElementChild->Attribute("height"));
		
		m_TileSetList.push_back(tempTileSet);

		if (xmlElement->NextSiblingElement("tileset") != NULL)
			xmlElement = xmlElement->NextSiblingElement();
		else
			xmlElement = NULL;
	}

	xmlElement = rootElement->FirstChildElement("layer");

	while (xmlElement)
	{
		Layer tempLayer;

		tempLayer.layerName = xmlElement->Attribute("name");
		tempLayer.layerWidth = atoi(xmlElement->Attribute("width"));
		tempLayer.layerHeight = atoi(xmlElement->Attribute("height"));

		tinyxml2::XMLElement* tempChild = xmlElement->FirstChildElement("data");

		tinyxml2::XMLElement* xmlElementChild = tempChild->FirstChildElement("tile");

		while (xmlElementChild)
		{
			tempLayer.data.push_back(atoi(xmlElementChild->Attribute("gid")));

			if (xmlElementChild->NextSiblingElement("tile") != NULL)
				xmlElementChild = xmlElementChild->NextSiblingElement();
			else
			{
				xmlElementChild = NULL;
				tempLayer.data.reserve(tempLayer.data.size());
			}
		}

		m_LayerList.push_back(tempLayer);

		if (xmlElement->NextSiblingElement("layer") != NULL)
			xmlElement = xmlElement->NextSiblingElement();
		else
			xmlElement = NULL;
	}

	xmlElement = rootElement->FirstChildElement("objectgroup");

	while (xmlElement)
	{
		ObjectGroup tempObjectGroup;

		tempObjectGroup.objectGroupName = xmlElement->Attribute("name");

		tinyxml2::XMLElement* xmlElementChild = xmlElement->FirstChildElement("object");
		tempObjectGroup.objectX = atoi(xmlElementChild->Attribute("x"));
		tempObjectGroup.objectY = atoi(xmlElementChild->Attribute("y"));
		tempObjectGroup.objectWidth = atoi(xmlElementChild->Attribute("width"));
		tempObjectGroup.objectHeight = atoi(xmlElementChild->Attribute("height"));

		m_ObjectGroupList.push_back(tempObjectGroup);

		if (xmlElement->NextSiblingElement("objectgroup") != NULL)
			xmlElement = xmlElement->NextSiblingElement();
		else
			xmlElement = NULL;
	}
}

void TileMap::InitializeTilemapBitmaps(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	int layerWidth, layerHeight;
	string filePath = "Resource Files/tileset/";
	vector<BitmapClass*> bitmapList;


	XMFLOAT2 tempuvs[4];

	tempuvs[BitmapClass::TOP_LEFT] = XMFLOAT2(0.0f, 0.0f);
	tempuvs[BitmapClass::TOP_RIGHT] = XMFLOAT2(1.0f, 0.0f);
	tempuvs[BitmapClass::BOTTOM_LEFT] = XMFLOAT2(0.0f, 1.0f);
	tempuvs[BitmapClass::BOTTOM_RIGHT] = XMFLOAT2(1.0f, 1.0f);

	for (unsigned int tilesets = 0; tilesets < m_TileSetList.size(); tilesets++)
	{
		BitmapClass* tempBitmap = new BitmapClass();
		tempBitmap->Initialize(device, deviceContext, (char*)string(filePath + m_TileSetList[tilesets].imageSource).c_str(), "Resource Files/myColor", 
								tempuvs, (float)m_TileSetList[tilesets].tileWidth, (float)m_TileSetList[tilesets].tileHeight);

		bitmapList.push_back(tempBitmap);
	}

	for (unsigned int layer = 0; layer < m_LayerList.size(); layer++)
	{
		layerWidth = m_LayerList[layer].layerWidth;
		layerHeight = m_LayerList[layer].layerHeight;

		for (int width = 0; width < layerWidth; width++)
		{
			for (int height = 0; height < layerHeight; height++)
			{
				Tile* tempTile = new Tile();

				tempTile->pos.x = (float)width;
				tempTile->pos.y = (float)height;

				int gID = m_LayerList[layer].data[width + (height * layerWidth)];
				int tilesetIndex;

				if (gID == 0)
				{
					delete tempTile;
					tempTile = NULL;
					continue;
				}

				for (unsigned int tileSet = 0; tileSet < m_TileSetList.size() - 1; tileSet++)
				{
					if (gID <= m_TileSetList[tileSet + 1].firstgid && gID >= m_TileSetList[tileSet].firstgid)
					{
						tilesetIndex = tileSet;
					}
				}

				int zeroBasedIndex = gID - m_TileSetList[tilesetIndex].firstgid;
				int tilesAmountX = m_TileSetList[tilesetIndex].imageWidth / m_TileSetList[tilesetIndex].tileWidth;
				int tilesAmountY = m_TileSetList[tilesetIndex].imageHeight / m_TileSetList[tilesetIndex].tileHeight;
				int indexX = zeroBasedIndex % tilesAmountX;
				int indexY = zeroBasedIndex / tilesAmountX;

				XMFLOAT2 uvs[4];

				uvs[BitmapClass::TOP_LEFT] = XMFLOAT2((float)indexX / tilesAmountX, (float)indexY / tilesAmountY);
				uvs[BitmapClass::TOP_RIGHT] = XMFLOAT2((float)(indexX + 1.0f) / tilesAmountX, (float)indexY / tilesAmountY);
				uvs[BitmapClass::BOTTOM_LEFT] = XMFLOAT2((float)indexX / tilesAmountX, (float)(indexY + 1.0f) / tilesAmountY);
				uvs[BitmapClass::BOTTOM_RIGHT] = XMFLOAT2((float)(indexX + 1.0f) / tilesAmountX, (float)(indexY + 1.0f) / tilesAmountY);

				tempTile->tileSetName = m_TileSetList[tilesetIndex].tileSetName;
				tempTile->bitmap = new BitmapClass();
				tempTile->bitmap->InitializeTilemapBitmap(*bitmapList[tilesetIndex], device, deviceContext, uvs, 
															(float)m_TileSetList[tilesetIndex].tileWidth, (float)m_TileSetList[tilesetIndex].tileHeight);
				tempTile->bitmap->SetPosition(XMFLOAT2(width * (float)m_TileSetList[tilesetIndex].tileWidth, -height * (float)m_TileSetList[tilesetIndex].tileHeight)); // Need to multiple position by half the dimensions of the tile
				tempTile->uvs[BitmapClass::TOP_LEFT] = uvs[BitmapClass::TOP_LEFT];
				tempTile->uvs[BitmapClass::TOP_RIGHT] = uvs[BitmapClass::TOP_RIGHT];
				tempTile->uvs[BitmapClass::BOTTOM_LEFT] = uvs[BitmapClass::BOTTOM_LEFT];
				tempTile->uvs[BitmapClass::BOTTOM_RIGHT] = uvs[BitmapClass::BOTTOM_RIGHT];

				switch (layer)
				{
					case BACKGROUND:
					{
						m_BackgroundTileList.push_back(tempTile);
					}
					break;
					case FOREGROUND:
					{
						m_ForegroundTileList.push_back(tempTile);
					}
					break;
					case TOP:
					{
						m_TopTileList.push_back(tempTile);
					}
					break;
				}
			}
		}
	}

	for (unsigned int i = 0; i < bitmapList.size(); i++)
	{
		delete bitmapList[i];
		bitmapList[i] = NULL;
	}
}

void TileMap::Shutdown()
{
	m_TileSetList.clear();
	m_LayerList.clear();
	m_ObjectGroupList.clear();

	for (unsigned int i = 0; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i]->Shutdown();
		delete m_ObjectList[i];
		m_ObjectList[i] = nullptr;
	}
	m_ObjectList.clear();

	for (unsigned int i = 0; i < m_BackgroundTileList.size(); i++)
	{
		m_BackgroundTileList[i]->bitmap->Shutdown();
		delete m_BackgroundTileList[i]->bitmap; 
		m_BackgroundTileList[i]->bitmap = nullptr;
		delete m_BackgroundTileList[i];
		m_BackgroundTileList[i] = nullptr;
	}
	m_BackgroundTileList.clear();

	for (unsigned int i = 0; i < m_ForegroundTileList.size(); i++)
	{
		m_ForegroundTileList[i]->bitmap->Shutdown();
		delete m_ForegroundTileList[i]->bitmap;
		m_ForegroundTileList[i]->bitmap = nullptr;
		delete m_ForegroundTileList[i];
		m_ForegroundTileList[i] = nullptr;
	}
	m_ForegroundTileList.clear();

	for (unsigned int i = 0; i < m_TopTileList.size(); i++)
	{
		m_TopTileList[i]->bitmap->Shutdown();
		delete m_TopTileList[i]->bitmap;
		m_TopTileList[i]->bitmap = nullptr;
		delete m_TopTileList[i];
		m_TopTileList[i] = nullptr;
	}
	m_TopTileList.clear();
}

void TileMap::Update(float fDt)
{

}

void TileMap::BackgroundForegroundRender(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projMatrix)
{
	for (unsigned int i = 0; i < m_BackgroundTileList.size(); i++)
	{
		m_BackgroundTileList[i]->bitmap->Render(deviceContext, viewMatrix, projMatrix);
	}

	for (unsigned int i = 0; i < m_ForegroundTileList.size(); i++)
	{
		m_ForegroundTileList[i]->bitmap->Render(deviceContext, viewMatrix, projMatrix);
	}
}

void TileMap::TopRender(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projMatrix)
{
	for (unsigned int i = 0; i < m_TopTileList.size(); i++)
	{
		m_TopTileList[i]->bitmap->Render(deviceContext, viewMatrix, projMatrix);
	}
}
