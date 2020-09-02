/************************************************************************************************************
* Avaac GL Simple Renderer																					*
*																											*
* This is a simple OpenGL library API providing 3D harwdare accelerated graphics capability for 2D game		*
* rendering																									*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/


#if (_WIN32)
	#include <SDL.h>
	#include <gl/glew.h>
	#include <gl/GL.h>
	#include <math.h>
#else
	#include <SDL/SDL.h>
	#include <GL/gl.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <dlfcn.h>
	#include <unistd.h>
	#include <string.h>
#endif

#include "agl_common.h"
#include "agl_resource.h"
#include "agl_vertex.h"
#include "agl_scene_graph.h"

// Static functions

static int __compare_z_layer_entry(const void *elem1, const void *elem2)
{
	static float base_pos = 0.0f;
	float key1_pos, key2_pos;
	AGL_ZLayerNode *key1, *key2;

	key1 = (AGL_ZLayerNode *) elem1;
	key2 = (AGL_ZLayerNode *) elem2;

	key1_pos = fabs(base_pos - key1->z_layer);
	key2_pos = fabs(base_pos - key2->z_layer);

	// Sort first by Z layer
	if (key1_pos < key2_pos)
		return 1;
	else if (key1_pos > key2_pos)
		return -1;
	else
		return 0;
}

static int32_t __get_z_node(AGL_SceneGraph *scene_graph, float z_layer)
{
	int32_t ctr = 0;
	while (ctr < scene_graph->node_count)
	{
		if (z_layer == scene_graph->scene_textures[ctr].z_layer)
			break;
		ctr++;
	}

	return ++ctr;
}

static int32_t __get_texture_node(AGL_ZLayerNode *z_node, AGL_Renderable *renderable)
{
	if (!renderable)
		return 0;

	// Check first to see the renderable is an image and if texture batching is enabled
	if (renderable->rc_type == RENDER_IMAGE)
	{
		//if (SGL_GetRenderingContext()->renderFlags & SGL_ENABLE_TEXTURE_BATCHING)
		//{
			int32_t ctr = 0;
			AGL_ImageData * image_data = (AGL_ImageData *) renderable->data_ptr;
			while (ctr < z_node->node_count)
			{
				if (image_data->texture_id == z_node->texture_nodes[ctr].texture_id)
					break;
				ctr++;
			}

			return ++ctr;
		//}
		//else
		//	return z_node->node_count + 1;
	}

	return 0;
}

// Public functions

void AGL_SceneGraph_DefaultSortScene(AGL_SceneGraph *scene_graph, AGL_ZLayerNode *node_list)
{
	qsort(node_list, scene_graph->node_count, sizeof(AGL_ZLayerNode), __compare_z_layer_entry);
}

void AGL_SceneGraph_AddRenderable(AGL_SceneGraph *scene_graph, AGL_Renderable *renderable, float *pos)
{
	// Get appropriate nodes - Check for existing Z node first, then check for extisting texture node
	int32_t z_cnt = __get_z_node(scene_graph, pos[2]);
	int32_t tex_cnt = __get_texture_node(&scene_graph->scene_textures[z_cnt - 1], renderable);
	int32_t tpos_cnt = scene_graph->scene_textures[z_cnt - 1].texture_nodes[tex_cnt - 1].node_count;

	// Assign new item in scene graph
	scene_graph->scene_textures[z_cnt - 1].z_layer = pos[2];
	AGL_TextureNode *new_node = &scene_graph->scene_textures[z_cnt - 1].texture_nodes[tex_cnt - 1];
	
	// Check render type
	if (renderable->rc_type == RENDER_IMAGE)
	{
		AGL_ImageData *image = (AGL_ImageData *) renderable->data_ptr;
		new_node->texture_id = image->texture_id;
		new_node->texture_pos[tpos_cnt].pos[0] = pos[0];
		new_node->texture_pos[tpos_cnt].pos[1] = pos[1];
		new_node->texture_pos[tpos_cnt].rc_type = RENDER_IMAGE;
		memcpy(&new_node->texture_pos[tpos_cnt].image, image, sizeof(AGL_ImageData));
	}

	// Check and update node counts
	if (z_cnt > scene_graph->node_count)
		scene_graph->node_count = z_cnt;
	if (tex_cnt > scene_graph->scene_textures[z_cnt - 1].node_count)
		scene_graph->scene_textures[z_cnt - 1].node_count = tex_cnt;
	scene_graph->scene_textures[z_cnt - 1].texture_nodes[tex_cnt - 1].node_count += 1;
}

void AGL_SceneGraph_ClearScene(AGL_SceneGraph *scene_graph)
{
	int32_t i, j;

	for (i = 0; i < scene_graph->node_count; i++)
	{
		AGL_ZLayerNode *z_node = &scene_graph->scene_textures[i];

		for (j = 0; j < z_node->node_count; j++)
			z_node->texture_nodes[j].node_count = 0;

		scene_graph->scene_textures[i].node_count = 0;
	}

	scene_graph->initialised = 0;
	scene_graph->node_count = 0;
}
