#ifndef _TILE_MAP_H_
#define _TILE_MAP_H_

#include <vector>
#include <algorithm>
using namespace std;
#include "../Utility Files/tinyxml2.h"
#include "../Graphics Files/Bitmapclass.h"
#include "../Object Files/BaseObject.h"

class TileMap
{
private:
	struct TileSet
	{
		int firstgid;
		string tileSetName;
		int tileWidth;
		int tileHeight;
		string imageSource;
		int imageWidth;
		int imageHeight;
	};

	struct Layer
	{
		string layerName;
		int layerWidth;
		int layerHeight;

		vector<int> data;
	};

	struct ObjectGroup
	{
		string objectGroupName;
		int objectX;
		int objectY;
		int objectWidth;
		int objectHeight;
	};

	struct Tile
	{
		XMFLOAT2 uvs[4];
		XMFLOAT2 pos;
		string tileSetName;
		BitmapClass* bitmap;
	};

	enum ORIENTATION { ORTHOGONAL, ISOMETRIC };
	enum LAYER { BACKGROUND, FOREGROUND, TOP};
	float m_version;
	ORIENTATION m_orientation;
	int m_tileMapWidth;
	int m_tileMapHeight;
	int m_tileWidth;
	int m_tileHeight;
	vector<TileSet>		m_TileSetList;
	vector<Layer>		m_LayerList;
	vector<ObjectGroup> m_ObjectGroupList;
	vector<BaseObject*> m_ObjectList;
	vector<Tile*>		m_BackgroundTileList;
	vector<Tile*>		m_ForegroundTileList;
	vector<Tile*>		m_TopTileList;

	void LoadTMXFile(char* fileName);
	void InitializeTilemapBitmaps(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

public:
	TileMap();
	TileMap(const TileMap& other);
	~TileMap();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int posX, int posY);
	void Shutdown();

	void Update(float fDt);
	void BackgroundForegroundRender(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projMatrix);
	void TopRender(ID3D11DeviceContext* deviceContext, XMMATRIX& viewMatrix, XMMATRIX& projMatrix);

	vector<BaseObject*> GetObjectList() { return m_ObjectList; }
};

#endif