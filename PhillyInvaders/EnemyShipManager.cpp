/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#include "common.h"
#include "EnemyShipManager.h"
#include "EnemyShip.h"

#define ENEMY_SHIPS_PER_ROW				19
#define ENEMY_SHIP_ROW_START_X			165
#define ENEMY_SHIP_ROW_START_Y			75
#define ENEMY_SHIP_ROW_COUNT			4
#define ENEMY_SHIP_GAP					10

EnemyShipManager::EnemyShipManager(ResourceManager* rc, Ship *player)
{
	ship_state = ENEMY_SHIP_STATE_INVALID;
	rc_manager = rc;
	player_ship.push_back(player);
}

EnemyShipManager::~EnemyShipManager()
{
	if (enemy_ships.size() > 0)
	{
		for (size_t i = 0; i < enemy_ships.size(); ++i)
		{
			if (enemy_ships[i])
				delete enemy_ships[i];
		}

		enemy_ships.clear();
	}
}

void EnemyShipManager::Initialize()
{
	// Lets start by creating all the enemy ships
	for (int i = 0; i < ENEMY_SHIP_ROW_COUNT; ++i)
	{
		for (int j = 0; j < ENEMY_SHIPS_PER_ROW; ++j)
		{
			EnemyShip* ship = new EnemyShip();
			int32_t ship_width = rc_manager->GetImageDataResource(ship->GetShipImageResource())->width;
			int32_t ship_height = rc_manager->GetImageDataResource(ship->GetShipImageResource())->height;
			int32_t x_pos = ENEMY_SHIP_ROW_START_X + (j * ship_width) + ENEMY_SHIP_GAP;
			int32_t y_pos = ENEMY_SHIP_ROW_START_Y + (i * ship_height) + ENEMY_SHIP_GAP;
			ship->Initialize(x_pos, y_pos, 100, 200, 1.0f, ship_width, ship_height);
			enemy_ships.push_back(ship);
		}
	}

	ship_state = ENEMY_SHIP_STATE_LEFT;
}

void EnemyShipManager::Update(float delta_time)
{
	if (enemy_ships.size() > 0)
	{
		// Base positional movement off first and last enemy ship in rows depending on state. This assumes there is at
		// least one row of ships, should probably update to make it dynamic, but that would only be useful with the
		// scenario config system
		int32_t left_ship_pos_x = enemy_ships[0]->GetShipPosX();	// First ship is all the way on the left
		int32_t right_ship_pos_x = enemy_ships[ENEMY_SHIPS_PER_ROW - 1]->GetShipPosX();	// Last ship in row
		int32_t left_ship_width = rc_manager->GetImageDataResource(enemy_ships[0]->GetShipImageResource())->width;
		int32_t right_ship_width = rc_manager->GetImageDataResource(enemy_ships[ENEMY_SHIPS_PER_ROW - 1]->GetShipImageResource())->width;

		switch (ship_state)
		{
		case ENEMY_SHIP_STATE_LEFT:
			if (left_ship_pos_x > (left_ship_width / 2))
			{
				for (size_t i = 0; i < enemy_ships.size(); ++i)
				{
					if (enemy_ships[i])
					{
						enemy_ships[i]->SetPosition((enemy_ships[i]->GetShipPosX() - (75 * delta_time)), enemy_ships[i]->GetShipPosY());
						enemy_ships[i]->Update(nullptr, delta_time, player_ship);	// Enemy ships don't require an input handler
					}
				}
			}
			else
			{
				ship_state = ENEMY_SHIP_STATE_RIGHT;
			}
			break;
		case ENEMY_SHIP_STATE_RIGHT:
			if (right_ship_pos_x < (GAME_WIDTH - (right_ship_width / 2)))
			{
				for (size_t i = 0; i < enemy_ships.size(); ++i)
				{
					if (enemy_ships[i])
					{
						enemy_ships[i]->SetPosition((enemy_ships[i]->GetShipPosX() + (150 * delta_time)), enemy_ships[i]->GetShipPosY());
						enemy_ships[i]->Update(nullptr, delta_time, player_ship);
					}
				}
			}
			else
			{
				ship_state = ENEMY_SHIP_STATE_LEFT;
			}
			break;
		case ENEMY_SHIP_STATE_INVALID:
		default:
			break;
		}
	}
}

void EnemyShipManager::Render(ResourceManager* rc_manager, float z_layer)
{
	for (size_t i = 0; i < enemy_ships.size(); ++i)
	{
		if (enemy_ships[i])
		{
			enemy_ships[i]->Render(rc_manager, z_layer);
		}
	}
}

std::vector<Ship*> EnemyShipManager::GetEnemyShipList()
{
	return enemy_ships;
}