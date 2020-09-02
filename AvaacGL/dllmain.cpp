/************************************************************************************************************
* Avaac GL Simple Renderer																					*
*																											*
* This is a simple OpenGL library API providing 3D harwdare accelerated graphics capability for 2D game		*
* rendering																									*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
