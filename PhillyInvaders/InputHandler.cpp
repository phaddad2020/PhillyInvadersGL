/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#include <stdint.h>

#include "InputHandler.h"

InputHandler::InputHandler()
{

}

InputHandler::~InputHandler()
{

}

void InputHandler::Update()
{
	while (SDL_PollEvent(&in_event));
}

int InputHandler::IsKeyDown(SDLKey key)
{
	int ret = 0;

	switch (in_event.type)
	{
	case SDL_KEYDOWN:
		if (in_event.key.keysym.sym == key)
			ret = 1;
		break;
	default:
		break;
	}

	return ret;
}