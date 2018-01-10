#pragma once
#include "SDL.h"

class Constants
{
public:
	static int MAX_PLAYERS_PER_ROOM_EX_LOCAL;
	static int MAX_PLAYERS;
	static const int MAX_PLAYERS_PER_SHIP;
	static const int MAX_NUM_OF_ROWS;
	static const int MAX_NUM_OF_COLS;
	static const int TILE_SIZE;
	static const SDL_Point OFFSET_COLLISION;
	static const int NUM_OF_GAME_COINS;
	static const SDL_Point cannonBallSize;
	static const SDL_Point playerSize;
	static const float _ENGINE_REPAIR_AMOUNT;
	static const float _BUTTON_COUNT_PER_ENGINE_SEQUENCE;
	static const int CENTRE_OF_WORLD;
	static const int SIZE_OF_WORLD;
	static const int MAX_WORLD_TILES_ROWS;
	static const int MAX_WORLD_TILES_COLS;
	static const int BUTTON_COUNT_TO_FIX_RUDDER;
	static const int NUM_AI_BOATS;
	static const int MAX_ENTITIES;
	static const int CANNON_BALL_SIZE;
	static const int MIRROR_X_POS;
	static const int MAX_NUM_OF_COINS;
	static const int _Coin_Size;
	static const int ANGLE_NEEDED_FOR_AI_TO_TURNWHEEL;
	static const int MAX_COIN_VALUE;
	static const int NUM_OF_CANNONS;
	static const float TIME_BETWEEN_AI_INPUT;
	static const float RANDOM_VARIATION_BETWEEN_INPUT;
	static float GetHorizontalIntersectionDepth(SDL_Rect & rectA, SDL_Rect & rectB);
	static float GetVerticalIntersectionDepth(SDL_Rect & rectA, SDL_Rect & rectB);

};


