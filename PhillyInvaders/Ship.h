/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __SHIP_H__
#define __SHIP_H__

#include <vector>

#include "ResourceManager.h"
#include "InputHandler.h"
#include "Projectile.h"

class Ship
{
public:
	Ship();
	virtual ~Ship();

	virtual void Initialize(int32_t pos_x, int32_t pos_y, int32_t health, 
							int32_t move_per_sec, float move_speed,
							int32_t width, int32_t height);
	virtual void Update(InputHandler* input_handler, float delta_time, std::vector<Ship*> ship_list);
	virtual void Render(ResourceManager* rc_manager, float z_layer);
	virtual void SetPosition(int32_t x_pos, int32_t y_pos);
	virtual void SetActive(bool set_active);
	virtual bool ShipHit(int32_t x_pos, int32_t y_pos);
	virtual int32_t GetShipImageResource() = 0;

	int32_t GetShipPosX();
	int32_t GetShipPosY();

protected:

	void addBullet(int32_t projectile_img_idx, eProjectDir dir);
	void destroyBullet(size_t index);

	std::vector<Projectile*> ship_bullets;
	int32_t ship_pos_x;
	int32_t ship_pos_y;
	int32_t ship_health;
	int32_t ship_move_per_sec;
	int32_t ship_width;
	int32_t ship_height;
	float ship_move_speed;
	bool active;
};

#endif