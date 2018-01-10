#include "stdafx.h"
#include "JsonLoader.h"
#include <iostream>

std::vector<std::vector<std::pair<int, int>>> JsonLoader::loadTileIds(std::string filename)
{
	std::vector<std::vector<std::pair<int, int>>> m_tileData(Constants::MAX_NUM_OF_ROWS, std::vector<std::pair<int, int>>(Constants::MAX_NUM_OF_COLS));
	
	//std::vector<std::vector<int>> m_tileIds(Constants::MAX_TILES_ROWS, std::vector<int>(Constants::MAX_TILES_COLS));
	if (fopen_s(&m_file, filename.c_str(), "rb") != 0)
	{
		std::cout << "Error opening file '" << &filename << "'" << std::endl;
	}
	else
	{
		FileReadStream inputStream(m_file, buffer, sizeof(buffer));
		m_data.ParseStream<0, UTF8<>, FileReadStream>(inputStream);

		for (int row = 0;  row < Constants::MAX_NUM_OF_ROWS; row++)
		{
			for (int  col = 0; col < Constants::MAX_NUM_OF_COLS; col++)
			{
				m_tileData[row][col].first = m_data["id"].GetArray()[row][col].GetInt();
				m_tileData[row][col].second = m_data["collidable"].GetArray()[row][col].GetInt();
			}
		}	
	}
	return m_tileData;
}

std::vector<std::vector<std::pair<int, int>>> JsonLoader::loadGameWorld(std::string filename)
{
	std::vector<std::vector<std::pair<int, int>>> m_tileData(Constants::MAX_WORLD_TILES_ROWS, std::vector<std::pair<int, int>>(Constants::MAX_WORLD_TILES_COLS));

	if (fopen_s(&m_file, filename.c_str(), "rb") != 0)
	{
		std::cout << "Error opening file '" << &filename << "'" << std::endl;
	}
	else
	{
		FileReadStream inputStream(m_file, buffer, sizeof(buffer));
		m_data.ParseStream<0, UTF8<>, FileReadStream>(inputStream);

		for (int row = 0; row < Constants::MAX_WORLD_TILES_ROWS; row++)
		{
			for (int col = 0; col < Constants::MAX_WORLD_TILES_COLS; col++)
			{
				m_tileData[row][col].first = m_data["id"].GetArray()[row][col].GetInt();
				m_tileData[row][col].second = m_data["collidable"].GetArray()[row][col].GetInt();
			}
		}
	}
	return m_tileData;
}
std::vector<std::pair<Vector2, int>> JsonLoader::loadEntities(std::string filename)
{
	
	std::vector<std::pair<Vector2, int>> m_entityData(Constants::MAX_ENTITIES);
	if (fopen_s(&m_file, filename.c_str(), "rb") != 0)
	{
		std::cout << "Error opening file '" << &filename << "'" << std::endl;
	}
	else
	{
		FileReadStream inputStream(m_file, buffer, sizeof(buffer));
		m_data.ParseStream<0, UTF8<>, FileReadStream>(inputStream);

		for (int i = 0; i < Constants::MAX_ENTITIES; i++)
		{
			m_entityData[i].first = Vector2(m_data["position"].GetArray()[i].GetArray()[0].GetInt(),m_data["position"].GetArray()[i].GetArray()[1].GetInt());
			m_entityData[i].second = m_data["textureID"].GetArray()[i][0].GetInt();
		}
	}
	return m_entityData;
}

std::vector<Vector2> JsonLoader::loadCoinData(std::string filename)
{
	std::vector<Vector2>m_coinData(Constants::MAX_NUM_OF_COINS);

	
	if (fopen_s(&m_file, filename.c_str(), "rb") != 0)
	{
		std::cout << "Error opening file '" << &filename << "'" << std::endl;
	}
	else
	{
		FileReadStream inputStream(m_file, buffer, sizeof(buffer));
		m_data.ParseStream<0, UTF8<>, FileReadStream>(inputStream);

		for (int i = 0; i < Constants::MAX_NUM_OF_COINS; i++)
		{
				m_coinData[i] = Vector2(m_data["position"].GetArray()[i].GetArray()[0].GetInt(), m_data["position"].GetArray()[i].GetArray()[1].GetInt());
		}
	}
	return m_coinData;
}
