#include "stdafx.h"
#include "PathFinder.h"
#include <algorithm>
#include "Entity.h"
#include "Constants.h"
#include "TileComponent.h"
#include "Identifiers.h"
#include <map>
#include "PositionComponent.h"

PathFinder * PathFinder::_instance;

PathFinder::PathFinder()
{
}

PathFinder * PathFinder::instance()
{
	if(_instance == nullptr)
	{
		_instance = new PathFinder();
	}

	return _instance;
}

PathFinder::~PathFinder()
{
}

Node * GetNodeInMap(map<int, Node *> * pNodeMap, SDL_Point pPos)
{
	int key = (pPos.y * Constants::MAX_NUM_OF_COLS) + pPos.x;

	if (pNodeMap->find(key) == pNodeMap->end())
	{
		Node * node = new Node();
		node->_posInGrid = pPos;
		pNodeMap->insert(make_pair(key, node));
	}

	return pNodeMap->at(key);
}

vector<Node> PathFinder::FindPathToIndex(SDL_Point pPos, SDL_Point pGoal, vector<vector<Entity>> * pTileMap)
{
	_positionInGrid = pPos;

	map<int, Node *> nodeMap = map<int, Node *>();

	//the list of nodes we still have to check out
	vector<Node*> openList = vector<Node*>();
	//the list of nodes we have checked
	vector<Node*> closedList = vector<Node*>();

	//find variables of starting tile	
	Node * node = GetNodeInMap(&nodeMap, _positionInGrid);
	node->_g = 0;
	node->_h = CalculateHeuristic(node->_posInGrid, pGoal);
	node->_f = node->_g + node->_h;
	openList.push_back(node);

	while (openList.size() != 0)
	{
		Node * currNode;

		for (int i = 0; i < openList.size(); i++)
		{
			currNode = openList[i];
			currNode->_h = CalculateHeuristic(currNode->_posInGrid, pGoal);
			currNode->_f = currNode->_g + (currNode->_h * _heuristicModifier);
		}

		int indexOfLowestF = -1;
		int lowestF = 99999999;
		for (int i = 0; i < openList.size(); i++)
		{
			//find the closest node
			if (openList[i]->_f < lowestF)
			{
				lowestF = openList[i]->_f;
				indexOfLowestF = i;
			}
		}

		//set current to the closest node
		Node * current = openList[indexOfLowestF];
		//if the goal
		if (current == GetNodeInMap(&nodeMap, pGoal))
		{
			return FindPath(current, nodeMap);
		}

		//remove current
		openList.erase(openList.begin() + indexOfLowestF);
		closedList.push_back(current);


		vector<Node*> neighbours = FindNeighbours(current, &nodeMap, pTileMap);

		for (int i = 0; i < neighbours.size(); i++)
		{
			Node * currNeighbour = neighbours.at(i);

			//if current is not in closedList
			if (find(closedList.begin(), closedList.end(), currNeighbour) == closedList.end())
			{
				currNeighbour->_h = CalculateHeuristic(currNeighbour->_posInGrid, pGoal);
				currNeighbour->_f = currNeighbour->_g + currNeighbour->_h;

				if (find(openList.begin(), openList.end(), currNeighbour) == openList.end())
				{
					openList.push_back(currNeighbour);
				}
				else
				{
					Node * comparerNode;
 					for (int j = 0; j < neighbours.size(); j++)
					{
						comparerNode = neighbours.at(j);
						if (comparerNode->_g < currNeighbour->_g && comparerNode->_parentNode != currNeighbour)
						{
							currNeighbour->_g = comparerNode->_g;
							currNeighbour->_parentNode = comparerNode;
						}
					}
				}
			}
		}
	}

	map<int, Node *>::iterator it = nodeMap.begin();
	if (it != nodeMap.end())
	{
		delete it->second;
		nodeMap.erase(it);
	}
	//no path found
	return vector<Node>();
}

int PathFinder::CalculateHeuristic(SDL_Point pPosInGrid, SDL_Point pGoal)
{
	int xDiff = max(pPosInGrid.x - pGoal.x, pGoal.x - pPosInGrid.x);
	int yDiff = max(pPosInGrid.y - pGoal.y, pGoal.y - pPosInGrid.y);

	//manhattan distance
	return (xDiff + yDiff) * 3;
}

vector<Node> PathFinder::FindPath(Node* pStartingNode, map<int, Node *> pMap)
{
	vector<Node> path = vector<Node>();

	path.push_back(*pStartingNode);

	Node * currNode = pStartingNode;

	while (currNode->_parentNode != nullptr && path.size() < 3000)
	{
		path.push_back(*currNode->_parentNode);
		currNode = currNode->_parentNode;
	}

	for (map< int, Node* >::iterator it = pMap.begin(); it != pMap.end(); ++it) {
		Node* point = it->second;
		delete point;
	}
	pMap.clear();

	if (path.size() > 1)
		return path;
	else
		return vector<Node>();
}

vector<Node*> PathFinder::FindNeighbours(Node * pParentNode, map<int, Node *> * pMap, vector<vector<Entity>> * pTileMap)
{
	vector<Node *> neighbours = vector<Node *>();
	SDL_Point parentIndex = pParentNode->_posInGrid;


	if (parentIndex.x == 27)
		int j = 0;

	if (pTileMap->at(parentIndex.y).at(parentIndex.x).getComponent<TileComponent>(ComponentTypes::Tile)->m_collidable)
		return neighbours;

	//checy for walls here
	Node * rightNode = GetNodeInMap(pMap,	SDL_Point{ parentIndex.x + 1, parentIndex.y });
	Node * leftNode = GetNodeInMap(pMap,	SDL_Point{ parentIndex.x - 1, parentIndex.y });
	Node * upNode = GetNodeInMap(pMap,		SDL_Point{ parentIndex.x, parentIndex.y + 1});
	Node * downNode = GetNodeInMap(pMap,	SDL_Point{ parentIndex.x, parentIndex.y - 1});

	if (parentIndex.x + 1 < Constants::MAX_NUM_OF_COLS && pTileMap->at(parentIndex.y).at(parentIndex.x + 1).getComponent<TileComponent>(ComponentTypes::Tile)->m_collidable == false)
		neighbours.push_back(rightNode);
	else
		pTileMap->at(parentIndex.y)[parentIndex.x + 1].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.y += 45;

	if (parentIndex.x - 1 >= 0 && pTileMap->at(parentIndex.y).at(parentIndex.x - 1).getComponent<TileComponent>(ComponentTypes::Tile)->m_collidable == false)
		neighbours.push_back(leftNode);
	if (parentIndex.y + 1 < Constants::MAX_NUM_OF_ROWS && pTileMap->at(parentIndex.y + 1).at(parentIndex.x).getComponent<TileComponent>(ComponentTypes::Tile)->m_collidable == false)
		neighbours.push_back(upNode);
	if (parentIndex.y - 1 >= 0 && pTileMap->at(parentIndex.y - 1).at(parentIndex.x).getComponent<TileComponent>(ComponentTypes::Tile)->m_collidable == false)
		neighbours.push_back(downNode);

	for (int i = 0; i < neighbours.size(); i++)
	{
		if (neighbours[i]->_parentNode == nullptr && !(neighbours[i]->_posInGrid.y == _positionInGrid.y && neighbours[i]->_posInGrid.x == _positionInGrid.x))
		{
			neighbours[i]->_g = pParentNode->_g + 1;
			neighbours[i]->_parentNode = pParentNode;
		}
	}

	return neighbours;
}
