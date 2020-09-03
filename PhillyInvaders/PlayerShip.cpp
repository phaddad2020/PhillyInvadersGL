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
#include "PlayerShip.h"

#define PLAYER_SHIP_SHOOT_INTERVAL_TIME	333333	// This is in microseconds (shoot 3 times a second)

PlayerShip::PlayerShip()
{
	shoot_start = 0;
	current_time = 0;
}

PlayerShip::~PlayerShip()
{

}

void PlayerShip::Initialize(int32_t pos_x, int32_t pos_y, int32_t health, 
						    int32_t move_per_sec, float move_speed,
							int32_t width, int32_t height)
{
	Ship::Initialize(pos_x, pos_y, health, move_per_sec, move_speed, width, height);
	shoot_start = 0;
	current_time = 0;
}

void PlayerShip::Update(InputHandler *input_handler, float delta_time, std::vector<Ship*> ship_list)
{
	if (input_handler)
	{
		if (input_handler->IsKeyDown(SDLK_d))
		{
			ship_pos_x += (int32_t)(ship_move_per_sec * delta_time);

			if (ship_pos_x >= (GAME_WIDTH - (ship_width / 2)))
				ship_pos_x = (GAME_WIDTH - (ship_width / 2));
		}

		if (input_handler->IsKeyDown(SDLK_a))
		{
			ship_pos_x -= (int32_t)(ship_move_per_sec * delta_time);

			if (ship_pos_x <= (ship_width / 2))
				ship_pos_x = (ship_width / 2);
		}

		if (input_handler->IsKeyDown(SDLK_SPACE))
		{
			if (shoot_start == 0)
			{
				shoot_start = STimer_GetTime();
				addBullet(IMAGE_PLAYER_PROJECTILE, PROJECTILE_DIR_UP);
			}
		}
	}

	// Player has triggered a shot
	if (shoot_start > 0)
	{
		// Update timings
		current_time = STimer_GetTime();
		if (current_time >= (shoot_start + PLAYER_SHIP_SHOOT_INTERVAL_TIME))
			shoot_start = 0;
	}

	// Make sure we call base update for projectiles
	Ship::Update(input_handler, delta_time, ship_list);
}

void PlayerShip::Render(ResourceManager *rc_manager, float z_layer)
{
	Ship::Render(rc_manager, z_layer);

	// Render any after effects here
}

int32_t PlayerShip::GetShipImageResource()
{
	return IMAGE_PLAYER_SHIP;
}