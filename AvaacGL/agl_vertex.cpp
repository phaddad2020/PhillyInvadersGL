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
#include "agl_vertex.h"
#include "agl_resource.h"

// Global statics
static AGL_ImageNode *image_nodes;
static AGL_Vertex *vertices;
static int32_t vertex_count, vertex_size;

// Static functions

/*
 *	agl_pot32
 *
 *	Simple function to return a new size based on the size passed in. We shift by 5 bits
 *	at a time to gaurantee a size will be returned thats always divisible by 32
 */
static int32_t agl_pot32(int32_t n)
{
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	return ++n;
}

// Public functions

/*
 *	AGL_VertexGetVertices
 *
 *	Function that returns array of vertices generated from all textures loaded
 *	Once a VBO is created from this data it can be safely removed
 */
AGL_Vertex *AGL_VertexGetVertices()
{
	return vertices;
}

/*
 *	AGL_VertexGetVertexCount
 *
 *	Function to return vertex count
 */
int32_t AGL_VertexGetVertexCount()
{
	return vertex_count;
}

/*
 *	AGL_VertexGetVertexStart
 *
 *	Function that takes image data and attempts to fill a vertex array to later be uploaded
 *	to the GPU via a vertex buffer object. It also returns an index as to where this specific
 *	image with its dimensions can be found in the array for rendering later on.
 *	It checks against data redundancy and ensures that if the same vertex data is already
 *	located in the array then it will simply return that start index rather than add duplicate
 *	data
 */
GLuint AGL_VertexGetVertexStart(AGL_ImageData *image_data)
{
	AGL_ImageNode *node = image_nodes;

	// Check to see if the vertex data already exists in the image node list
	for (node = image_nodes; node; node = node->next)
	{
		if (image_data->width == node->width && image_data->height == node->height)
			return node->vertex_start;
	}

	// Vertex data for this particular resource doesnt exist so lets add it
	node				= (AGL_ImageNode *) malloc(sizeof(AGL_ImageNode));
	node->width			= image_data->width;
	node->height		= image_data->height;
	node->vertex_start	= vertex_count;
	node->next			= image_nodes;
	image_nodes			= node;

	// Check to see if vertex size needs to be increased. This is padded by 4 bytes (32 bits) at a time
	if (vertex_count + 4 > vertex_size)
	{
		vertex_size = agl_pot32(vertex_count + 20);
		vertices = (AGL_Vertex *) realloc(vertices, vertex_size * sizeof(AGL_Vertex));
	}

	// Assign vertex data to vertice array which will be used to fill a vbo later
	AGL_Vertex *v = &vertices[vertex_count];

	vertex_count += 4;

	v[0].tx = 0;
	v[0].ty = (float) image_data->height;
	v[1].tx = (float) image_data->width;
	v[1].ty = (float) image_data->height;
	v[2].tx = (float) image_data->width;
	v[2].ty = 0;
	v[3].tx = 0;
	v[3].ty = 0;

	float l, t;

	l = -(float) image_data->width / (float) AGL_GetRenderingContext()->width / 2;
	t = (float) image_data->height / (float) AGL_GetRenderingContext()->height / 2;

	v[0].x = l;
	v[0].y = -t;
	v[1].x = -l;
	v[1].y = -t;
	v[2].x = -l;
	v[2].y = t;
	v[3].x = l;
	v[3].y = t;

	return node->vertex_start;
}

void AGL_VertexRefillTextureData(AGL_ImageData *image_data)
{
	if (!image_data)
		return;

	// For the moment we are going to ignore reproduction of data. This is inefficient however
	// it will be reduced by the dormant nature of fbo usage, it is rare that you will get
	// reproducible data in an atlas map situation but still possible. Need to come back
	// to this and design a proper vertex array handler (this is an example of where c++
	// container classes can come in handy)

	AGL_ImageNode *node = image_nodes;

	for (node = image_nodes; node; node = node->next)
	{
		// Do nothing
	}

	node				= (AGL_ImageNode *) malloc(sizeof(AGL_ImageNode));
	node->width			= image_data->width;
	node->height		= image_data->height;
	node->vertex_start	= vertex_count;
	node->next			= image_nodes;
	image_nodes			= node;

	// Check to see if vertex size needs to be increased. This is padded by 4 bytes (32bits) at a time
	if (vertex_count + 4 > vertex_size)
	{
		vertex_size = agl_pot32(vertex_count + 20);
		vertices = (AGL_Vertex *) realloc(vertices, vertex_size * sizeof(AGL_Vertex));
	}

	// Assign vertex data tpo vertice array which will be used to fill a vbo later
	AGL_Vertex *v = &vertices[vertex_count];

	vertex_count += 4;

	v[0].tx = image_data->vertex[0].tx;
	v[0].ty = image_data->vertex[0].ty;
	v[1].tx = image_data->vertex[1].tx;
	v[1].ty = image_data->vertex[1].ty;
	v[2].tx = image_data->vertex[2].tx;
	v[2].ty = image_data->vertex[2].ty;
	v[3].tx = image_data->vertex[3].tx;
	v[3].ty = image_data->vertex[3].ty;

	v[0].x = image_data->vertex[0].x;
	v[0].y = image_data->vertex[0].y;
	v[1].x = image_data->vertex[1].x;
	v[1].y = image_data->vertex[1].y;
	v[2].x = image_data->vertex[2].x;
	v[2].y = image_data->vertex[2].y;
	v[3].x = image_data->vertex[3].x;
	v[3].y = image_data->vertex[3].y;

	image_data->vertex_start = node->vertex_start;
}
