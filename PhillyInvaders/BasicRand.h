/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __BASIC_RAND_H__
#define __BASIC_RAND_H__

#include <ctime>
#include <cstdlib>

typedef int base_rand;

void BaseRand_Init();
base_rand BaseRand_GetRandMax(int max);

#endif