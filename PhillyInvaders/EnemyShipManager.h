/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __ENEMY_SHIP_MANAGER_H__
#define __ENEMY_SHIP_MANAGER_H__

#include <vector>

#include "Ship.h"

#define ENEMY_SHIP_STATE_INVALID	0
#define ENEMY_SHIP_STATE_LEFT		1
#define ENEMY_SHIP_STATE_RIGHT		2

/// <summary>
/// This class managers all enemy ships. The idea here is you would be able to load a separately configured
/// and maintained scenario which could have a collection of specific enemy type ships. For now just going
/// basic all the same
/// </summary>
class EnemyShipManager
{
public:
	EnemyShipManager(ResourceManager *rc, Ship *player);
	~EnemyShipManager();

	// This function would take a configuration object which would initialize a specific scenario
	void Initialize();
	void Update(float delta_time);
	void Render(ResourceManager* rc_manager, float z_layer);

	std::vector<Ship*> GetEnemyShipList();

private:
	ResourceManager* rc_manager;
	std::vector<Ship*> enemy_ships;
	std::vector<Ship*> player_ship;
	int32_t ship_state;
};

#endif