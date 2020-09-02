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

#include "Ship.h"

Ship::Ship()
{
	ship_pos_x = ship_pos_y = 0;
	ship_health = 0;
	ship_move_speed = 0.0f;
	ship_move_per_sec = 0;
	active = false;
}

Ship::~Ship()
{
	for (size_t i = 0; i < ship_bullets.size(); ++i)
	{
		if (ship_bullets[i])
			delete ship_bullets[i];
	}

	ship_bullets.clear();
}

void Ship::Initialize(int32_t pos_x, int32_t pos_y, int32_t health, 
					  int32_t move_per_sec, float move_speed,
					  int32_t width, int32_t height)
{
	ship_pos_x = pos_x;
	ship_pos_y = pos_y;
	ship_health = health;
	ship_move_per_sec = move_per_sec;
	ship_move_speed = move_speed;
	ship_width = width;
	ship_height = height;
	active = true;	// Activate is assumed upon initialization
}

void Ship::Update(InputHandler* input_handler, float delta_time, std::vector<Ship*> ship_list)
{
	for (size_t i = 0; i < ship_bullets.size(); ++i)
	{
		if (ship_list.size() > 0)
		{
			for (size_t j = 0; j < ship_list.size(); ++j)
			{
				if (ship_list[j]->ShipHit(ship_bullets[i]->GetPosX(), ship_bullets[i]->GetPosY()))
				{
					std::cout << "SHIP HIT" << std::endl;
					destroyBullet(i);
					ship_list[j]->SetActive(false);
					// This bullet is destroyed so we leave the loop. Of course we would want to revisit this
					// in the case of special bullets that would allow a single bullet to destroy multiple ships
					break;
				}
			}
		}

		// If we have previously destroyed the only bullet in the list don't continue execution. This may seem strange
		// but covers a corner case because it's possible to have between 3 and 4 bullets from any ship on the screen
		// at any given time due to the shoot interval
		if (ship_bullets.size() > 0)
		{
			if (ship_bullets[i] && ship_bullets[i]->Update(delta_time))
			{
				destroyBullet(i);
			}
		}
	}
}

void Ship::Render(ResourceManager* rc_manager, float z_layer)
{
	if (!rc_manager)
		return;

	if (active)
		AGL_RenderImage(rc_manager->GetImageDataResource(GetShipImageResource()), ship_pos_x, ship_pos_y, z_layer);

	for (size_t i = 0; i < ship_bullets.size(); ++i)
	{
		ship_bullets[i]->Render(rc_manager);
	}
}

void Ship::SetPosition(int32_t x_pos, int32_t y_pos)
{
	ship_pos_x = x_pos;
	ship_pos_y = y_pos;
}

void Ship::SetActive(bool set_active)
{
	active = set_active;
}

bool Ship::ShipHit(int32_t x_pos, int32_t y_pos)
{
	if (active)
	{
		if (x_pos >= ship_pos_x && x_pos <= ship_pos_x + ship_width)
		{
			if (y_pos >= ship_pos_y && y_pos <= ship_pos_y + ship_health)
			{
				return true;
			}
		}
	}

	return false;
}

int32_t Ship::GetShipPosX()
{
	return ship_pos_x;
}

int32_t Ship::GetShipPosY()
{
	return ship_pos_y;
}

void Ship::addBullet(int32_t projectile_img_idx)
{
	Projectile* new_bullet = new Projectile(projectile_img_idx, ship_pos_x, ship_pos_y);
	ship_bullets.push_back(new_bullet);
}

void Ship::destroyBullet(size_t index)
{
	if (index < ship_bullets.size() && ship_bullets[index])
	{
		delete ship_bullets[index];
		ship_bullets.erase(ship_bullets.begin() + index);
		std::cout << "Projectile destroyed" << std::endl;
	}
}

