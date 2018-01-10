#pragma once
#include "InputHandler.h"
#include "BaseSystem.h"
#include "Entity.h"
#include "CollisionComponent.h"
#include "TileComponent.h"

enum Direction
{
	Horizontal,
	Vertical,
};

class CollisionSystem : public BaseSystem
{
public:
	void handleCollisions(std::vector<std::vector<Entity>> & tileEntities)
	{
		for (size_t i = 0; i < m_entities.size(); i++)
		{
			PositionComponent * pc = m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position);
			CollisionComponent * cc = m_entities[i]->getComponent<CollisionComponent>(ComponentTypes::Collision);

			if (pc->m_previousPosition.x != pc->m_position.x)
			{
				cc->m_collisionBounds.x = pc->m_position.x + (Constants::OFFSET_COLLISION.x * 0.5f) - Constants::OFFSET_COLLISION.x;	// Set CollisionBounds X Position
				cc->m_collisionBounds.y = pc->m_previousPosition.y + Constants::OFFSET_COLLISION.y;		// Set Y position to the previous players Y position "Seperate Axis, X axis first"
				handleCollisionAxis(Direction::Horizontal, tileEntities, pc, cc);						// Handle X Axis Collision
			}
			if (pc->m_previousPosition.y != pc->m_position.y)
			{

				cc->m_collisionBounds.x = pc->m_position.x + (Constants::OFFSET_COLLISION.x * 0.5f) - Constants::OFFSET_COLLISION.x;

				cc->m_collisionBounds.y = pc->m_position.y + Constants::OFFSET_COLLISION.y;				// Set CollisionBounds X Position
				handleCollisionAxis(Direction::Vertical, tileEntities, pc, cc);							// Handle X Axis Collision
			}
		}
	}
	void handleCollisionAxis(Direction direction, std::vector<std::vector<Entity>> & tileEntities, PositionComponent * pc, CollisionComponent * cc)
	{
		int leftTile = (cc->m_collisionBounds.x) / Constants::TILE_SIZE;
		int topTile = cc->m_collisionBounds.y / Constants::TILE_SIZE;
		int rightTile = (int)ceil((float)(cc->m_collisionBounds.x + Constants::OFFSET_COLLISION.x) / Constants::TILE_SIZE) - 1;
		int bottomTile = (int)ceil(((float)(cc->m_collisionBounds.y + Constants::OFFSET_COLLISION.y) / Constants::TILE_SIZE)) - 1;

		for (int y = topTile; y <= bottomTile; ++y)
		{
			for (int x = leftTile; x <= rightTile; ++x)
			{
				if (direction == Direction::Horizontal)	// X Axis
				{
					TileComponent * tileC = tileEntities[y][x].getComponent<TileComponent>(ComponentTypes::Tile);
					CollisionComponent * tileCC = tileEntities[y][x].getComponent<CollisionComponent>(ComponentTypes::Collision);
					if (tileC->m_collidable)
					{
						float depth = Constants::GetHorizontalIntersectionDepth(cc->m_collisionBounds, tileCC->m_collisionBounds);
						if (depth < 0)
							pc->m_position.x = tileCC->m_collisionBounds.x - Constants::TILE_SIZE + (Constants::OFFSET_COLLISION.x * 0.5f);
						else if (depth > 0)
							pc->m_position.x = tileCC->m_collisionBounds.x + Constants::TILE_SIZE - (Constants::OFFSET_COLLISION.x * 0.5f);
					}
					cc->m_collisionBounds.x = pc->m_position.x + (Constants::OFFSET_COLLISION.x * 0.5f);
				}
				else if (direction == Direction::Vertical)	// Y Axis
				{
					TileComponent * tileC = tileEntities[y][x].getComponent<TileComponent>(ComponentTypes::Tile);
					CollisionComponent * tileCC = tileEntities[y][x].getComponent<CollisionComponent>(ComponentTypes::Collision);
					if (tileC->m_collidable)
					{
						float depth = Constants::GetVerticalIntersectionDepth(cc->m_collisionBounds, tileCC->m_collisionBounds);
						if (depth < 0)
							pc->m_position.y = tileCC->m_collisionBounds.y - Constants::TILE_SIZE;
						else
							pc->m_position.y = tileCC->m_collisionBounds.y + Constants::TILE_SIZE - Constants::OFFSET_COLLISION.y;
					}
				}
			}
		}
	}
};


