/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#include <stdint.h>

#include "common.h"
#include "agl_renderer.h"
#include "resources_enum.h"

#include "Projectile.h"

#define PROJECTILE_MOVE_SPEED_PER_SEC	750

Projectile::Projectile(int32_t image_idx, int32_t start_pos_x, int32_t start_pos_y, eProjectDir dir, int32_t dmg, int32_t speed)
{
	pos_x = start_pos_x;
	pos_y = start_pos_y;
	move_speed_per_sec = speed;
	rc_idx = image_idx;
	proj_dir = dir;
	proj_speed = speed;
	proj_damage = dmg;
}

Projectile::~Projectile()
{

}

int Projectile::Update(float delta_time)
{
	if (proj_dir == PROJECTILE_DIR_UP)
	{
		pos_y -= (int32_t)(move_speed_per_sec * delta_time);

		if (pos_y <= 0)
			return 1;
	}
	else
	{
		pos_y += (int32_t)(move_speed_per_sec * delta_time);

		if (pos_y >= (GAME_HEIGHT - 5))
			return 1;
	}

	return 0;
}

void Projectile::Render(ResourceManager *rc_manager)
{
	if (!rc_manager)
		return;

	AGL_RenderImage(rc_manager->GetImageDataResource(rc_idx), pos_x, pos_y, -0.4f);
}

int32_t Projectile::GetPosX()
{
	return pos_x;
}

int32_t Projectile::GetPosY()
{
	return pos_y;
}

int32_t Projectile::GetDamage()
{
	return proj_damage;
}