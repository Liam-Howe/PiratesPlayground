#pragma once
#include <string>
#include <vector>
#include "Constants.h"
#include "dependencies/rapidjson/document.h"
#include "dependencies/rapidjson/filereadstream.h"
#include "Vector2.h"
using namespace rapidjson;
class JsonLoader
{
public:
	std::vector<std::vector<std::pair<int, int>>> loadTileIds(std::string);
	std::vector<std::vector<std::pair<int, int>>> loadGameWorld(std::string);
	std::vector<std::pair<Vector2, int>> JsonLoader::loadEntities(std::string filename);
	std::vector<Vector2> loadCoinData(std::string filename);
	Document m_data;
	FILE* m_file;
	char buffer[65535];
};