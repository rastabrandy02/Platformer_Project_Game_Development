
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// L06: TODO 7: Ask for the value of a custom property
int Properties::GetProperty(const char* value, int defaultValue) const
{
	//...

	ListItem<Property*>* item = list.start;
	

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return defaultValue;
}


// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

    return ret;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	
	if (mapLoaded == false) return;

	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.mapLayers.start;

	// L06: TODO 4: Make sure we draw all the layers and not just the first one
	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->properties.GetProperty("Draw") == 1) {

			for (int x = 0; x < mapLayerItem->data->width; x++)
			{
				for (int y = 0; y < mapLayerItem->data->height; y++)
				{
					// L04: DONE 9: Complete the draw function
					int gid = mapLayerItem->data->Get(x, y);

					if (gid > 0) {

						//L06: TODO 4: Obtain the tile set using GetTilesetFromTileId
						//now we always use the firt tileset in the list
						//TileSet* tileset = mapData.tilesets.start->data;
						TileSet* tileset = GetTilesetFromTileId(gid);

						SDL_Rect r = tileset->GetTileRect(gid);
						iPoint pos = MapToWorld(x, y);

						app->render->DrawTexture(tileset->texture,
							pos.x,
							pos.y,
							&r);
					}

				}
			}
		}
		

		mapLayerItem = mapLayerItem->next;
	}
	
}




// L04: TODO 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * mapData.tileWidth;
	ret.y = y * mapData.tileHeight;

	return ret;
}

// L05: DON 2: Add orthographic world to map coordinates
iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	// L05: DONE 3: Add the case for isometric maps to WorldToMap
	/*if (mapData.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / mapData.tileWidth;
		ret.y = y / mapData.tileHeight;
	}
	else if (mapData.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = mapData.tileWidth * 0.5f;
		float half_height = mapData.tileHeight * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}*/
	ret.x = x / mapData.tileWidth;
	ret.y = y / mapData.tileHeight;
	return ret;
}

// L06: TODO 3: Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = mapData.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

// L04: TODO 7:Implement the method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };
	int relativeIndex = id - firstgid;

	rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
	rect.y = margin + (tileHeight + spacing) * (relativeIndex / columns);
	rect.w = tileWidth;
	rect.h = tileHeight;

	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	bool ret = true; 

    LOG("Unloading map");

    // L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
    // Remove all tilesets
	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.Clear();

	// L04: TODO 2: clean up all layer data
	// Remove all layers
	//mapData.maplayers.clear();
	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.mapLayers.start;

	while (mapLayerItem != NULL)
	{
		RELEASE(mapLayerItem->data);
		mapLayerItem = mapLayerItem->next;
	}

	return true;

}

// Load new map
bool Map::Load(const char* filename)
{
    bool ret = true;
    SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_document mapFile; 
    pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
        ret = false;
    }

    if(ret == true)
    {
		ret = LoadMap(mapFile);
	}
	
    if (ret == true)
    {
        ret = LoadTileSets(mapFile.child("map"));
    }

	// L04: TODO 4: Iterate all layers and load each of them
	
	if (ret == true)
	{
		ret = LoadAllLayers(mapFile.child("map"));
	}

    if(ret == true)
    {
        // L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything


		// L04: TODO 4: LOG the info for each loaded layer
    }
	if (ret == true)
	{
		ret = SetMapColliders();
	}
	// Clean up the pugui tree
	if(mapFile) mapFile.reset();

    mapLoaded = ret;

    return ret;
}

// L03: DONE 3: Implement LoadMap to load the map properties
bool Map::LoadMap(pugi::xml_node mapFile)
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		// L03: DONE 3: Load map general properties
		mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
	}

	return ret;
}

// L03: DONE 4: Implement the LoadTileSet function to load the tileset properties
bool Map::LoadTileSets(pugi::xml_node mapFile) {

	bool ret = true;

	pugi::xml_node tileset;
	for (tileset = mapFile.child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();
		if (ret == true) ret = LoadTilesetDetails(tileset, set);
		if (ret == true) ret = LoadTilesetImage(tileset, set);
		mapData.tilesets.Add(set);
	}

	return ret;
}

// L03: TODO 4: Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

	// L03: DONE 4: Load Tileset attributes
	set->name.Create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
	set->tileHeight = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	set->tilecount = tileset_node.attribute("tilecount").as_int();
	set->columns = tileset_node.attribute("columns").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	return ret;
}

// L03: DONE 4: Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// L03: DONE 4: Load Tileset image
		SString tmp("%s%s", folder.GetString(), image.attribute("source").as_string());
		
		//SDL_Surface* surface = IMG_Load(tmp.GetString()); //Need include #include "SDL_image/include/SDL_image.h"
		//set->texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);

		//NOTE: This line simplifies the loading
		set->texture = app->tex->Load(tmp.GetString());
	}

	return ret;
}

// L04: TODO 3: Implement the LoadLayer function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	
	//Load the attributes and assign to the layer variables
	layer->id = node.attribute("id").as_int();
	layer->name = node.attribute("name").as_string();
	layer->height = node.attribute("height").as_int();
	layer->width = node.attribute("width").as_int();
	
	
	LoadProperties(node, layer->properties);

	//Initialize the tile array and reserve the memory 
	layer->data = new uint[layer->height * layer->width];
	memset(layer->data, 0, layer->height * layer->width);

	//Iterate over all the tiles in the xml and assign the values
	int i = 0;
	for (pugi::xml_node tileNode = node.child("data").child("tile"); tileNode; tileNode = tileNode.next_sibling("tile"))
	{
		layer->data[i] = tileNode.attribute("gid").as_int();
		i++;
	}

	return ret;
}

// L04: TODO 4: Iterate all layers and load each of them
// This function is call from Mapp::Load()
bool Map::LoadAllLayers(pugi::xml_node mapNode) {
	
	bool ret = true; 
	
	//Iterate ovel all layers in the XML and call the individual LoadLayer()
	for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
	{
		MapLayer* mapLayer = new MapLayer();
		LoadLayer(layerNode, mapLayer);
		mapData.mapLayers.Add(mapLayer);
	}
	return ret;
}
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
	{
		Properties::Property* p = new Properties::Property();
		p->name = propertieNode.attribute("name").as_string();
		p->value = propertieNode.attribute("value").as_int();

		properties.list.Add(p);
	}

	return ret;
}
bool Map::SetMapColliders()
{
	bool ret = true;

	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.mapLayers.start;
	LOG("--------!!!SETTING COLLIDERS!!!---------");
	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->properties.GetProperty("Collider") == 1) {

			for (int x = 0; x < mapLayerItem->data->width; x++)
			{
				for (int y = 0; y < mapLayerItem->data->height; y++)
				{
					int gid = mapLayerItem->data->Get(x, y);

					if (gid > 0) {

						TileSet* tileset = GetTilesetFromTileId(gid);

						SDL_Rect r = tileset->GetTileRect(gid);
						iPoint pos;
						pos = MapToWorld(x, y);
						PhysBody* item = app->physics->CreateStaticRectangle(pos.x + (tileset->tileWidth * 0.5f), pos.y + (tileset->tileHeight * 0.5f), tileset->tileWidth, tileset->tileHeight);
						if (mapLayerItem->data->properties.GetProperty("Death") == 1)
						{
							item->body->GetFixtureList()->SetUserData((void*)DATA_DEATH);
						}
						else item->body->GetFixtureList()->SetUserData((void*)DATA_GROUND);
						app->physics->groundColliders.Add(item);
						
					}

				}
			}
		}
		mapLayerItem = mapLayerItem->next;
	}
	return ret;
}

bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	ListItem<MapLayer*>* item;
	
	item = mapData.mapLayers.start;
	

	for (item = mapData.mapLayers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.GetProperty("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);

		for (int y = 0; y < mapData.height; ++y)
		{
			for (int x = 0; x < mapData.width; ++x)
			{
				int i = (y * layer->width) + x;

				int tileId = layer->Get(x, y);
				TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tileId - tileset->firstgid) > 0 ? 0 : 1;
				}
			}
		}

		*buffer = map;
		width = mapData.width;
		height = mapData.height;
		ret = true;

		break;
	}

	return ret;
}