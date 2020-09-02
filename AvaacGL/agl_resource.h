/************************************************************************************************************
* Avaac GL Simple Renderer																					*
*																											*
* This is a simple OpenGL library API providing 3D harwdare accelerated graphics capability for 2D game		*
* rendering																									*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __AGL_RESOURCE_H__
#define __AGL_RESOURCE_H__


#if (_WIN32)
	#include <SDL.h>
	#include <gl/glew.h>
	#include <gl\GL.h>
#else
	#include <SDL/SDL.h>
	#include <GL/gl.h>
#endif

#include <stdint.h>

#include "agl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *	AvaacGL Image Format Flags
 */
#define AGL_IMG_ALPHA	0x0100

/*
 *	AvaacGL Image Data struct
 *
 *	Structure which holds image resource data
 */
typedef struct agl_image_data_struct
{
	SDL_Surface		*image_surface;
	int32_t			texture_generated;
	GLuint			texture_id;
	int32_t			texture_flags;
	int32_t			texture_format;
	int32_t			width;
	int32_t			height;
	int32_t			bpp;
	float			alpha;
	AGL_Vertex		*vertex;
	GLuint			vertex_start;
} AGL_ImageData;

// Functions
extern SYMBOL_EXPORT AGL_ImageData *AGL_LoadImageDataFromFile(const char *filename);
extern SYMBOL_EXPORT AGL_ImageData *AGL_LoadTextureFromImage(AGL_ImageData *image_data);
extern SYMBOL_EXPORT AGL_ImageData *AGL_LoadTextureFromFile(const char *filename);
extern SYMBOL_EXPORT void AGL_DeleteTexture(AGL_ImageData *image_data);

#ifdef __cplusplus
}
#endif

#endif