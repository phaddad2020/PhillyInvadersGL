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
#include "EnemyShip.h"

EnemyShip::EnemyShip()
{

}

EnemyShip::~EnemyShip()
{

}

void EnemyShip::Initialize(int32_t pos_x, int32_t pos_y, int32_t health, 
						   int32_t move_per_sec, float move_speed,
						   int32_t width, int32_t height)
{
	Ship::Initialize(pos_x, pos_y, health, move_per_sec, move_speed, width, height);
}

void EnemyShip::Update(InputHandler* input_handler, float delta_time, std::vector<Ship*> ship_list)
{
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