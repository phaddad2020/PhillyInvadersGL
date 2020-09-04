/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __ENEMY_SHIP_H__
#define __ENEMY_SHIP_H__

#include <stdint.h>

#include "Ship.h"
#include "STimer.h"

/// <summary>
/// This is the base enemy ship controlling class.
/// </summary>
class EnemyShip : public Ship
{
public:
	EnemyShip();
	~EnemyShip();

	void Initialize(int32_t pos_x, int32_t pos_y, int32_t health, 
					int32_t move_per_sec, float move_speed,
					int32_t width, int32_t height) override;
	void Update(InputHandler* input_handler, float delta_time, std::vector<Ship*> ship_list) override;
	void Render(ResourceManager* rc_manager, float z_layer) override;

	int32_t GetShipImageResource() override;
	int32_t GetProjectileDamage() override;
	int32_t GetProjectileSpeed() override;

private:
	STimer start_time;
	int32_t fire_chance;

};

#endif