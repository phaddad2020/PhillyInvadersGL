/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __INPUT_HANDLER_H__
#define __INPUT_HANDLER_H__

#include <stdint.h>

#include <SDL.h>

/// <summary>
/// Very simple input handler which just polls upon request. This should probably be implemented properly
/// and wrap the underlying SDL layer in a way that works for this type of game. Trying to save time here
/// at the moment though
/// </summary>
class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	void Update();

	int IsKeyDown(SDLKey key);
private:
	SDL_Event in_event;
};

#endif
