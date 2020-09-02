/************************************************************************************************************
* Avaac GL Simple Renderer																					*
*																											*
* This is a simple OpenGL library API providing 3D harwdare accelerated graphics capability for 2D game		*
* rendering																									*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __AGL_SCENE_GRAPH_H__
#define __AGL_SCENE_GRAPH_H__

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
#include "agl_renderer.h"
#include "agl_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *	AvaacGL Texture Pos struct
 *
 *	Simple struct that holds positional information for a specific texture. This will
 *	most likely grow with more data the more advanced this renderer gets
 */
typedef struct agl_texture_pos
{
	float pos[2];
	int rc_type;
	AGL_ImageData image;

} AGL_TexturePos;

/*
 *	AvaacGL Texture Node struct
 *
 *	Struct which holds a texture and its vertex data for rendering
 *	an array of this struct is contained within a Z layer node which
 *	describes all the textures to presented on that Z layer
 */
typedef struct agl_texture_node
{
	GLuint texture_id;
	AGL_TexturePos texture_pos[100];
	int32_t node_count;
} AGL_TextureNode;

/*
 *	AvaacGL Z Layer Node struct
 *
 *	Struct which holds a list of texture nodes on the specified Z
 *	layer. There is currently a maximum of 50 individuals textures
 *	on a specific Z layer this will be optimized over time. This
 *	will also hold a possible movie which cannot be batched.
 */
typedef struct agl_z_layer_node
{
	float z_layer;
	int32_t node_count;
	AGL_TextureNode texture_nodes[100];
} AGL_ZLayerNode;

/*
 *	AvaacGL Scene Graph struct
 *
 *	Struct which is the root of a scene graph that contains a list of
 *	Z layer nodes which in turn holds texture data to be rendered to
 *	the scene
 */
typedef struct agl_scene_graph
{
	int32_t initialised;
	int32_t node_count;
	AGL_ZLayerNode scene_textures[200];
} AGL_SceneGraph;

// Public functions
extern SYMBOL_EXPORT void AGL_SceneGraph_DefaultSortScene(AGL_SceneGraph *scene_graph, AGL_ZLayerNode *node_list);
extern SYMBOL_EXPORT void AGL_SceneGraph_AddRenderable(AGL_SceneGraph *scene_graph, AGL_Renderable *renderable, float *pos);
extern SYMBOL_EXPORT void AGL_SceneGraph_ClearScene(AGL_SceneGraph *scene_graph);

#ifdef __cplusplus
}
#endif

#endif