/************************************************************************************************************
* Avaac GL Simple Renderer																					*
*																											*
* This is a simple OpenGL library API providing 3D harwdare accelerated graphics capability for 2D game		*
* rendering																									*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __AGL_VERTEX_H__
#define __AGL_VERTEX_H__

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

/*
 *	AvaacGL Image Node struct
 *
 *	Struct that defines image nodes to help create a vertex array
 *	while avoiding the reproduction of vertex data
 */
typedef struct agl_image_node
{
	struct agl_image_node *next;
	int32_t width;
	int32_t height;
	GLuint vertex_start;
} AGL_ImageNode;

// Functions
extern SYMBOL_EXPORT AGL_Vertex *AGL_VertexGetVertices();
extern SYMBOL_EXPORT int32_t AGL_VertexGetVertexCount();
extern SYMBOL_EXPORT GLuint AGL_VertexGetVertexStart(AGL_ImageData *image_data);
extern SYMBOL_EXPORT void AGL_VertexRefillTextureData(AGL_ImageData *image_data);

#ifdef __cplusplus
}
#endif

#endif