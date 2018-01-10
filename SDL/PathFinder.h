#pragma once
#include <vector>
#include "Node.h"
#include "Entity.h"
#include <map>

using namespace std;

class PathFinder
{
public:
	static PathFinder * instance();
	~PathFinder();

	vector<Node> FindPathToIndex(SDL_Point pPos, SDL_Point pGoal, vector<vector<Entity>>* pTileMap);

private:
	static PathFinder * _instance;
	PathFinder();
	int CalculateHeuristic(SDL_Point pPosInGrid, SDL_Point pGoal);
	vector<Node>  FindPath(Node* pStartingNode, map<int, Node *> pMap);
	vector<Node *> FindNeighbours(Node * pParentNode, map<int, Node *> * pMap, vector<vector<Entity>> * pTileMap);

	SDL_Point _positionInGrid;
	float _heuristicModifier = 1.5;
};
