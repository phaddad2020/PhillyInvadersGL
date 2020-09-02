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
#include "agl_renderer.h"
//#include "sgl_maths.h"

// Global statics
static AGL_SceneGraph scene_graph;
static GLuint vbo;

// Static functions

static void agl_render_image_retained(const AGL_ImageData *image_data)
{
	if (!AGL_GetRenderingContext() || !image_data)
		return;

	// Check first for alpha
	if (image_data->texture_flags & AGL_IMG_ALPHA)
	{
		if (image_data->alpha < 1.0f)
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glColor4f(1, 1, 1, 1.0f - image_data->alpha);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
		}
	}
	else
		glDisable(GL_BLEND);

	if (image_data->texture_id)
	{
		glDrawArrays(GL_TRIANGLE_FAN, image_data->vertex_start, 4);
	}

	if (image_data->texture_flags & AGL_IMG_ALPHA)
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glDisable(GL_BLEND);
	}
}

static void agl_add_renderable_to_scene(AGL_Renderable *renderable, float *pos)
{
	if (!scene_graph.initialised)
		scene_graph.initialised = 1;

	AGL_SceneGraph_AddRenderable(&scene_graph, renderable, pos);
}

static void agl_render_create_vbo()
{
	// Check to see if vbo has been created
	if (!vbo)
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER,
					 AGL_VertexGetVertexCount() * sizeof(AGL_Vertex),
					 AGL_VertexGetVertices(),
					 GL_STATIC_DRAW);
		fprintf(stderr, "VBO created! Size: %lu, Vertex Count: %d\n",
				AGL_VertexGetVertexCount() * sizeof(AGL_Vertex),
				AGL_VertexGetVertexCount());
		free(AGL_VertexGetVertices());
	}
}

static void agl_render_gl_start()
{
	// First sort the scene graph
	AGL_SceneGraph_DefaultSortScene(&scene_graph, &scene_graph.scene_textures[0]);

	agl_render_create_vbo();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(2, GL_FLOAT, sizeof(AGL_Vertex), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(AGL_Vertex), (void *) (2 * sizeof(float)));

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_RECTANGLE_ARB);
}

static void sgl_render_gl_end()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_RECTANGLE_ARB);
}

static void agl_render_texture_node(AGL_ImageData *image_data, AGL_TexturePos *pos, float z_layer)
{
	if (!image_data || !pos)
		return;

	float current_pos[3];
	current_pos[0] = pos->pos[0];
	current_pos[1] = pos->pos[1];
	current_pos[2] = z_layer;

	glPushMatrix();
	glTranslatef(current_pos[0],
				 current_pos[1],
				 current_pos[2]);

	agl_render_image_retained(image_data);

	glPopMatrix();
}

static void agl_render_scene()
{
	if (!scene_graph.initialised)
		return;

	agl_render_gl_start();

	int32_t z_ctr = 0;
	
	// Loop through scene graph starting with Z layer
	while (z_ctr < scene_graph.node_count)
	{
		int32_t texture_node_count = scene_graph.scene_textures[z_ctr].node_count;
		int32_t texture_ctr = 0;
		float z_layer = scene_graph.scene_textures[z_ctr].z_layer;
		while (texture_ctr < texture_node_count)
		{
			AGL_TextureNode *tex_node = &scene_graph.scene_textures[z_ctr].texture_nodes[texture_ctr];
			GLuint texture_id = tex_node->texture_id;

			int32_t max_tex_pos_ctr = scene_graph.scene_textures[z_ctr].texture_nodes[texture_ctr].node_count;
			int32_t tex_pos_ctr = 0;

			// Bind this texture only once to render all positions it exists in
			if (texture_id != 0 && tex_node->texture_pos[tex_pos_ctr].rc_type == RENDER_IMAGE)
				glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture_id);

			while (tex_pos_ctr < max_tex_pos_ctr)
			{
				AGL_TexturePos *tex_pos = &tex_node->texture_pos[tex_pos_ctr];
				if (tex_pos->rc_type == RENDER_IMAGE)
				{
					agl_render_texture_node(&tex_pos->image, tex_pos, z_layer);
					tex_pos_ctr++;
				}

			}
			texture_ctr++;
		}
		z_ctr++;
	}

	sgl_render_gl_end();
}

void AGL_RenderImage(AGL_ImageData *image, int32_t x, int32_t y, float z)
{
	float pos[3];
	pos[0] = AGL_Get3DCoord_X(x);
	pos[1] = AGL_Get3DCoord_Y(y);
	pos[2] = z;
	AGL_Renderable renderable;
	renderable.rc_type = RENDER_IMAGE;
	renderable.data_ptr = image;
	agl_add_renderable_to_scene(&renderable, pos);
}

void AGL_SwapBuffers()
{
	agl_render_scene();

	SDL_GL_SwapBuffers();

	AGL_SceneGraph_ClearScene(&scene_graph);
}


