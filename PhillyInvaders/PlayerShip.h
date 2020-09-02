/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __PLAYER_SHIP_H__
#define __PLAYER_SHIP_H__

#include <stdint.h>

#include "Ship.h"
#include "STimer.h"

#define PLAYER_SHIP_Z_LAYER					-0.5f
#define PLAYER_SHIP_START_POS_X				640
#define PLAYER_SHIP_START_POS_Y				740
#define PLAYER_SHIP_MOVE_PER_SEC			350
#define PLAYER_SHIP_HEALTH					100

/// <summary>
/// This is the main player ship controlling class
/// </summary>
class PlayerShip : public Ship
{
public:
	PlayerShip();
	~PlayerShip();

	void Initialize(int32_t pos_x, int32_t pos_y, int32_t health, 
					int32_t move_per_sec, float move_speed,
					int32_t width, int32_t height) override;
	void Update(InputHandler* input_handler, float delta_time, std::vector<Ship*> ship_list) override;
	void Render(ResourceManager* rc_manager, float z_layer) override;

	int32_t GetShipImageResource() override;

private:
	STimer shoot_start;
	STimer current_time;
};

#endif