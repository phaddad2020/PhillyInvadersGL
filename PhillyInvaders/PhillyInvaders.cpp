/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#include <memory>
#include <iostream>
#include <stdint.h>

#include "common.h"
#include "BasicRand.h"
#include "PhillyInvaders.h"
#include "resources_enum.h"

#define GAME_TITLE			"PhillyInvaders"
#define RC_FILE				"list_resources.txt"

PhillyInvaders::PhillyInvaders()
{
	render_context = nullptr;
	rc_manager = nullptr;
	input_handler = nullptr;
	player_ship = nullptr;
	enemy_ship_manager = nullptr;
}

PhillyInvaders::~PhillyInvaders()
{
	if (render_context)
		free(render_context);

	if (rc_manager)
		rc_manager->DestroyResources();
}

int PhillyInvaders::Initialize()
{
	if (render_context)
		return -1;

	render_context = AGL_CreateRenderingContext(0, GAME_WIDTH, GAME_HEIGHT, GAME_TITLE);
	if (!render_context)
		return -2;

	rc_manager = std::make_unique<ResourceManager>();

	rc_manager->LoadResources(RC_FILE);

	// Initialize basic RNG
	BaseRand_Init();

	input_handler = std::make_unique<InputHandler>();
	player_ship = std::make_unique<PlayerShip>();

	if (player_ship)
	{
		int32_t ship_width = rc_manager->GetImageDataResource(player_ship->GetShipImageResource())->width;
		int32_t ship_height = rc_manager->GetImageDataResource(player_ship->GetShipImageResource())->height;
		player_ship->Initialize(PLAYER_SHIP_START_POS_X,
			PLAYER_SHIP_START_POS_Y,
			PLAYER_SHIP_HEALTH,
			PLAYER_SHIP_MOVE_PER_SEC,
			1.0f,
			ship_width,
			ship_height);
	}

	enemy_ship_manager = std::make_unique<EnemyShipManager>(rc_manager.get(), player_ship.get());
	if (enemy_ship_manager)
	{
		enemy_ship_manager->Initialize();
	}

	start_time = STimer_GetTime();
	frame_time_start = frame_time_end = 0;
	delta_time = 0.0f;

	return 0;	// All Good
}

int PhillyInvaders::Run()
{
	frame_time_start = STimer_GetTime();
	if (!updateGame())
		return 0;

	renderGame();
	frame_time_end = STimer_GetTime();

	// Calculate time related data, consider corner case like startup it's possible to be instant and we dont want to divide by zero
	// I should probably contain this as an object and pass that around instead
	if (frame_time_end > frame_time_start)
		fps = 1000000 / (frame_time_end - frame_time_start);
	else
		fps = 60;	// There is incredibly low texture bandwidth or vertices being used in this application lets assume 60 FPS at the worst

	delta_time = (float) (1 / (float)fps);
	//std::cout << "TIMER-- start: " << start_time << " frame_start: " << frame_time_start << ", frame_end: " << frame_time_end << " diff: " 
	//	<< (frame_time_end - frame_time_start) << " fps: " << fps << " delta: " << delta_time << std::endl;

	return 1;
}

ResourceManager* PhillyInvaders::GetRCManager()
{
	if (rc_manager)
		return rc_manager.get();
	return nullptr;
}

InputHandler* PhillyInvaders::GetInputHandler()
{
	if (input_handler)
		return input_handler.get();
	return nullptr;
}

float PhillyInvaders::GetDeltaTime()
{
	return delta_time;
}

int PhillyInvaders::updateGame()
{
	input_handler->Update();

	if (player_ship)
		player_ship->Update(input_handler.get(), delta_time, enemy_ship_manager->GetEnemyShipList());

	if (enemy_ship_manager)
		enemy_ship_manager->Update(delta_time);

	if (input_handler->IsKeyDown(SDLK_q))
		return 0;

	return 1;	// Continue running
}

void PhillyInvaders::renderGame()
{
	// Render the background
	AGL_RenderImage(rc_manager->GetImageDataResource(IMAGE_SPACE_BACKGROUND), 640, 400, -0.7f);

	if (player_ship)
		player_ship->Render(rc_manager.get(), -0.5f);

	if (enemy_ship_manager)
		enemy_ship_manager->Render(rc_manager.get(), -0.5f);

	AGL_SwapBuffers();
}
