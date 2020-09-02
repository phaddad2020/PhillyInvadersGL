/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include <stdint.h>
#include <vector>
#include <functional>

#include "ResourceManager.h"

class Projectile
{
public:
	Projectile(int32_t image_idx, int32_t start_pos_x, int32_t start_pos_y);
	~Projectile();

	int Update(float delta_time);
	void Render(ResourceManager *rc_manager);

	int32_t GetPosX();
	int32_t GetPosY();

private:
	int32_t pos_x;
	int32_t pos_y;
	int32_t move_speed_per_sec;
	int32_t rc_idx;
};

#endif