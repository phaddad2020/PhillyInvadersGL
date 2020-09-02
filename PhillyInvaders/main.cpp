/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#if (_WIN32)

#include <Windows.h>

#endif

#include <memory>
#include <iostream>
#include <stdint.h>

#include "PhillyInvaders.h"

// Legacy SDL redefines main to be SDL_main which is annoying
#undef main

int main(int argc, char* argv[])
{
	// It's cool making game with my name in it
	std::unique_ptr<PhillyInvaders> pInvadersGame;

	pInvadersGame = std::make_unique<PhillyInvaders>();

	if (pInvadersGame->Initialize())
	{
		std::cout << "[ERROR]: Game cannot be initialized" << std::endl;
		return 1;
	}

	while (pInvadersGame->Run())
	{

	}

	return 0;
}