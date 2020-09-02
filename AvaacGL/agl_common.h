/************************************************************************************************************
* Avaac GL Simple Renderer																					*
*																											*
* This is a simple OpenGL library API providing 3D harwdare accelerated graphics capability for 2D game		*
* rendering																									*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __AGL_COMMON_H__
#define __AGL_COMMON_H__

#if (_WIN32)
	#include <SDL.h>
	#include <gl/glew.h>
	#include <gl/GL.h>
	#include <gl/GLU.h>
	#include <stdint.h>
#else
	#include <SDL/SDL.h>
	#include <GL/gl.h>
	#include <stdint.h>
#endif

#if (_WIN32)

#define SYMBOL_EXPORT __declspec(dllexport)

#else

#define SYMBOL_EXPORT 

#endif

#ifdef __cplusplus
extern "C" {
#endif

// GL Error Check
int agl_ErrorCheck(const char *GLcall, const char *file, int line);
#define AGL_GL_ERROR_CHECK(msg)					\
{													\
	agl_ErrorCheck(#msg, __FILE__, __LINE__);	\
}

// This is what I use for the standard 2D ortho
#define AGL_VIEW_X		0.5
#define AGL_VIEW_Y		0.5
#define AGL_VIEW_Z_NEAR	0.0
#define AGL_VIEW_Z_FAR	1.0

/*
 *	The following are Avaac GL defines which determine how Avaac GL will render
 *	graphics
 */
#define AGL_ENABLE_FULLSCREEN					(1 << 4)	// etc ..


typedef enum
{
	AGL_RET_SUCCESS,
	AGL_RET_FAILURE
} returnCode_e;

/*
 *	Avaac GL Vertex Data base struct
 *
 *	Structure to hold very basic vertex data for 2D rendering
 */
typedef struct agl_vec2D
{
	float x;
	float y;
	float tx;
	float ty;
} AGL_Vertex;

/*
 *	Avaac GL Rendering Context struct
 *
 *	Structure which holds a rendering context for the entire application
 */
typedef struct agl_rendering_context
{
	int32_t		width;
	int32_t		height;
	int32_t		renderFlags;
	int32_t		sdlFlags;
	int32_t		perf_stats;
	char		*title;
	SDL_Surface	*render_surface;
} AGL_RenderingContext;

/*
 *	Avaac GL Open GL Info struct
 *
 *	Struct containing important information about OpenGL and the hardware that
 *	Avaac GL will be running on
 */
typedef struct agl_opengl_info
{
	const GLubyte *renderer;
	const GLubyte *vendor;
	const GLubyte *version;
	const GLubyte *glslVersion;
	GLint glslMajor;
	GLint glslMinor;
} AGL_OpenGLInfo;

// Functions
extern SYMBOL_EXPORT AGL_RenderingContext *AGL_CreateRenderingContext(int32_t render_flags, int32_t width, int32_t height,
													   const char *title);
extern SYMBOL_EXPORT AGL_RenderingContext *AGL_GetRenderingContext();
extern SYMBOL_EXPORT AGL_OpenGLInfo *AGL_GetOpenGLInfo();
extern SYMBOL_EXPORT void AGL_QueryOpenGLInfo(AGL_OpenGLInfo *opengl_info);

#ifdef __cplusplus
}
#endif

#endif