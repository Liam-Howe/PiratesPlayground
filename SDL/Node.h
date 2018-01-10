#pragma once
#include <SDL.h>

//the node in the Navigation Mesh - contains a map of the nodes attached to each thread 
struct Node
{
	SDL_Point _posInGrid;

	// g rating - distance in nodes of how far we've come so far
	int _g;

	// h rating - heuristic of how far the node is away from the goal
	int _h;

	// f rating - the h + the g rating combined
	int _f;

	//the previous node in the path to the goal
	Node* _parentNode;
};
