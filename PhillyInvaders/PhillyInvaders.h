/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __PHILLY_INVADERS_H__
#define __PHILLY_INVADERS_H__

#include <memory>

#include <stdint.h>

#include <agl_common.h>
#include <agl_renderer.h>

#include "ResourceManager.h"
#include "InputHandler.h"
#include "PlayerShip.h"
#include "EnemyShipManager.h"
#include "STimer.h"

/// <summary>
/// This is the main class of the game which acts as a container and the base for the application to update
/// render and check for global input
/// </summary>
class PhillyInvaders
{
public:
	PhillyInvaders();
	~PhillyInvaders();

	int Initialize();

	/// <summary>
	/// This function will update and render the entire game. It only returns upon exit/quit detect
	/// </summary>
	int Run();

	ResourceManager* GetRCManager();
	InputHandler* GetInputHandler();
	float GetDeltaTime();

private:

	int updateGame();
	void renderGame();

	AGL_RenderingContext* render_context;
	std::unique_ptr<ResourceManager> rc_manager;
	std::unique_ptr<InputHandler> input_handler;
	std::unique_ptr<PlayerShip> player_ship;
	std::unique_ptr<EnemyShipManager> enemy_ship_manager;

	// Timer related data
	STimer start_time;
	STimer frame_time_start;
	STimer frame_time_end;
	float delta_time;
	int fps;
};

#endif