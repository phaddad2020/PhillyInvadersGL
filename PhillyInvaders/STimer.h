/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __STIMER_H__
#define __STIMER_H__

#include <stdint.h>
#include <SDL.h>

// Doing a super simple timer here with 32bit integer consistency
typedef int32_t STimer;

#define STIMER_SECONDS(s)			((STimer) (s * 1000))
#define STIMER_MSECS(s)				(s)

/// <summary>
/// This will use SDL to return a timer in microseconds
/// </summary>
static inline STimer STimer_GetTime() { return SDL_GetTicks() * 1000; }

#endif