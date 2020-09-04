/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#include <iostream>

#include "agl_renderer.h"
#include "resources_enum.h"

#include "common.h"
#include "BasicRand.h"
#include "EnemyShip.h"

// Let's make it decide every second right now
#define ENEMY_SHIP_SHOOT_INTERVAL		1000000
#define ENEMY_SHIP_FIRE_CHANCE_DEFAULT	25	// Consider between 1 and 100 as percentage so 25% chance

EnemyShip::EnemyShip()
{
	start_time = 0;
	fire_chance = 0;
}

EnemyShip::~EnemyShip()
{

}

void EnemyShip::Initialize(int32_t pos_x, int32_t pos_y, int32_t health, 
						   int32_t move_per_sec, float move_speed,
						   int32_t width, int32_t height)
{
	Ship::Initialize(pos_x, pos_y, health, move_per_sec, move_speed, width, height);

	start_time = 0;
	fire_chance = ENEMY_SHIP_FIRE_CHANCE_DEFAULT;
}

void EnemyShip::Update(InputHandler* input_handler, float delta_time, std::vector<Ship*> ship_list)
{
	if (active)
	{
		if (start_time == 0)
		{
			start_time = STimer_GetTime() + ENEMY_SHIP_SHOOT_INTERVAL;
		}

		if (STimer_GetTime() > start_time)
		{
			start_time = 0;
			base_rand rand_result = BaseRand_GetRandMax(1000);
			if (rand_result <= fire_chance)
			{
				addBullet(IMAGE_ENEMY_PROJECTILE, PROJECTILE_DIR_DOWN);
			}
		}
	}

	// Make sure we call base update for projectiles. Also make sure we do it outside of active
	// flag check so we complete any projectile cycles
	Ship::Update(input_handler, delta_time, ship_list);
}

void EnemyShip::Render(ResourceManager* rc_manager, float z_layer)
{
	Ship::Render(rc_manager, z_layer);
}

int32_t EnemyShip::GetShipImageResource()
{
	return IMAGE_ENEMY_SHIP;
}

int32_t EnemyShip::GetProjectileDamage()
{
	return 25;
}

int32_t EnemyShip::GetProjectileSpeed()
{
	return 325;
}