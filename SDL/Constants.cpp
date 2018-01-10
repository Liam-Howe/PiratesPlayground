#include "stdafx.h"
#include "Constants.h"
#include <cmath>
int Constants::MAX_PLAYERS_PER_ROOM_EX_LOCAL = 2;
int Constants::MAX_PLAYERS = 0;
const int Constants::MAX_PLAYERS_PER_SHIP = 2;
const int Constants::MAX_NUM_OF_ROWS = 17;
const int Constants::MAX_NUM_OF_COLS = 29;
const int Constants::TILE_SIZE = 64;
const SDL_Point Constants::OFFSET_COLLISION{ 32,32 };

const SDL_Point Constants::cannonBallSize{ 32,32 };
const SDL_Point Constants::playerSize{ 64,64 };

const float Constants::_ENGINE_REPAIR_AMOUNT = 0.05f;
const float Constants::_BUTTON_COUNT_PER_ENGINE_SEQUENCE = 10;
const int Constants::_Coin_Size = 40;
const int Constants::CENTRE_OF_WORLD = 0;
const int Constants::SIZE_OF_WORLD = 500;
const int Constants::BUTTON_COUNT_TO_FIX_RUDDER = 20;
const int Constants::MAX_ENTITIES = 15;
const int Constants::MAX_WORLD_TILES_ROWS = 500;
const int Constants::MAX_WORLD_TILES_COLS = 17;
const int Constants::NUM_AI_BOATS = 2;
const int Constants::NUM_OF_CANNONS = 2;

const int Constants::CANNON_BALL_SIZE = 15;
const int Constants::MIRROR_X_POS = 1000;
const int Constants::MAX_NUM_OF_COINS = 15;
const int Constants::ANGLE_NEEDED_FOR_AI_TO_TURNWHEEL = 10;
const int Constants::MAX_COIN_VALUE = 50;
const float Constants::TIME_BETWEEN_AI_INPUT = 0.2f;
const float Constants::RANDOM_VARIATION_BETWEEN_INPUT = 0.5f;
const int Constants::NUM_OF_GAME_COINS = 5;
float Constants::GetHorizontalIntersectionDepth(SDL_Rect & rectA, SDL_Rect & rectB)
 {
	 	// Calculate half sizes.
	 	float halfWidthA = rectA.w / 2.0f;
	 	float halfWidthB = rectB.w / 2.0f;

	 	// Calculate centers.
	 	float centerA = rectA.x + halfWidthA;
	 	float centerB = rectB.x + halfWidthB;

	 	// Calculate current and minimum-non-intersecting distances between centers.
	 	float distanceX = centerA - centerB;
	 	float minDistanceX = halfWidthA + halfWidthB;

	 	// If we are not intersecting at all, return (0, 0).
	 	if (abs(distanceX) >= minDistanceX)
	 		return 0.0f;

	 	// Calculate and return intersection depths.
	 	if (distanceX > 0)
	 		return minDistanceX - distanceX;
	 	else
	 		return -minDistanceX - distanceX;
 }

 float Constants::GetVerticalIntersectionDepth(SDL_Rect & rectA, SDL_Rect & rectB)
 {
	 	// Calculate half sizes.
	 	float halfHeightA = rectA.h / 2.0f;
	 	float halfHeightB = rectB.h / 2.0f;

	 	// Calculate centers.
	 	float centerA = rectA.y + halfHeightA;
	 	float centerB = rectB.y + halfHeightB;

	 	// Calculate current and minimum-non-intersecting distances between centers.
	 	float distanceY = centerA - centerB;
	 	float minDistanceY = halfHeightA + halfHeightB;

	 	// If we are not intersecting at all, return (0, 0).
	 	if (abs(distanceY) >= minDistanceY)
	 		return 0.0f;

	 	// Calculate and return intersection depths.
	 	if (distanceY > 0)
	 		return minDistanceY - distanceY;
	 	else
	 		return -minDistanceY - distanceY;
	
 }
