/************************************************************************************************************
* Avaac GL Simple Renderer																					*
*																											*
* This is a simple OpenGL library API providing 3D harwdare accelerated graphics capability for 2D game		*
* rendering																									*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __AGL_RENDERER_H__
#define __AGL_RENDERER_H__


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
#include "agl_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum agl_render_type
{
	RENDER_IMAGE,
} AGL_RenderType;

typedef struct agl_renderable
{
	void * data_ptr;
	int rc_type;
} AGL_Renderable;

extern SYMBOL_EXPORT void AGL_RenderImage(AGL_ImageData *image, int32_t x, int32_t y, float z);
extern SYMBOL_EXPORT void AGL_SwapBuffers();

// inline utility functions for rendering
static inline float AGL_Get3DCoordFromLeft_X(int32_t x, int32_t w)
{
	return ((((float) (x + (float)w / 2.0f)) / AGL_GetRenderingContext()->width) - 0.5f);
}

static inline float AGL_Get3DCoordFromTop_Y(int32_t y, int32_t h)
{
	return (-((float) (y + (float) h / 2.0f) / AGL_GetRenderingContext()->height) + 0.5f);
}

static inline float AGL_Get3DCoord_X(int32_t x)
{
	return ((((float) (x)) / AGL_GetRenderingContext()->width) - 0.5f);
}

static inline float AGL_Get3DCoord_Y(int32_t y)
{
	return (-(((float) (y)) / AGL_GetRenderingContext()->height) + 0.5f);
}

static inline float AGL_Get3DCoordW_X(int32_t x, int32_t width)
{
	return ((((float) (x)) / width) - 0.5f);
}

static inline float AGL_Get3DCoordH_Y(int32_t y, int32_t height)
{
	return ((((float) (y)) / height) - 0.5f);
}

#ifdef __cplusplus
}
#endif

#endif