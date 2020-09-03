/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#include <iostream>

#include "BasicRand.h"

/// <summary>
/// This is very basic just seed the rand algorithm with a time value
/// </summary>
void BaseRand_Init()
{
	srand((unsigned int)time(0));
}

/// <summary>
/// Very simple function to return a number between 1 and the maximum passed in
/// </summary>
base_rand BaseRand_GetRandMax(int max)
{
	if (max > 1 && max < RAND_MAX)
	{
		return ((rand() % max) + 1);
	}

	return 1;
}