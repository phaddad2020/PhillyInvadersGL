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
	#include <SDL_image.h>
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

// Static functions

// Public functions
AGL_ImageData *AGL_LoadImageDataFromFile(const char *filename)
{
	SDL_RWops *rwops;
	AGL_ImageData *image_data = NULL;

	if (!AGL_GetRenderingContext())
		return NULL;

	if (filename)
	{
		fprintf(stderr, "Loading image %s\n", filename);
		image_data = (AGL_ImageData *) calloc(1, sizeof(AGL_ImageData));

		// Use SDL_Image to open the image and read it in as an SDL_Surface
		rwops = SDL_RWFromFile(filename, "r");

		if (rwops)
			image_data->image_surface = IMG_Load_RW(rwops, 1);

		if (!image_data->image_surface)
		{
			image_data->width = 0;
			image_data->height = 0;
			image_data->texture_id = 0;
			return image_data;
		}

		if (image_data->image_surface->format->Amask > 0)
			image_data->texture_flags |= AGL_IMG_ALPHA;
		else
			image_data->texture_flags &= ~AGL_IMG_ALPHA;

		image_data->width = image_data->image_surface->w;
		image_data->height = image_data->image_surface->h;
		image_data->texture_generated = 0;
		image_data->texture_id = 0;
	}

	return image_data;
}

AGL_ImageData *AGL_LoadTextureFromImage(AGL_ImageData *image_data)
{
	AGL_ImageData *new_image = NULL;

	GLubyte *pixel_data = (GLubyte *) image_data->image_surface->pixels;
	uint32_t alpha = (image_data->texture_flags & AGL_IMG_ALPHA);
	int32_t format = (alpha) ? GL_RGBA : GL_RGB;

	glEnable(GL_TEXTURE_RECTANGLE_ARB);

	if (image_data->texture_generated == 0)
	{
		glGenTextures(1, &image_data->texture_id);
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, image_data->texture_id);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB,
					 0,
					 format,
					 image_data->width,
					 image_data->height,
					 0,
					 format,
					 GL_UNSIGNED_BYTE,
					 pixel_data);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		image_data->texture_generated = 1;
	}
	else
	{
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, image_data->texture_id);
		glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB,
						0,
						0,
						0,
						image_data->width,
						image_data->height,
						format,
						GL_UNSIGNED_BYTE,
						pixel_data);
	}

	new_image = (AGL_ImageData *) calloc(1, sizeof(AGL_ImageData));
	new_image->vertex = (AGL_Vertex *) calloc(1, sizeof(AGL_Vertex) * 4);
	new_image->width = image_data->width;
	new_image->height = image_data->height;
	new_image->texture_id = image_data->texture_id;
	new_image->texture_flags = image_data->texture_flags;
	new_image->alpha = 0.0f;
	new_image->bpp = (image_data->texture_flags & AGL_IMG_ALPHA) ? 4 : 3;
	new_image->image_surface = NULL;
	SDL_FreeSurface(image_data->image_surface);
	image_data->image_surface = NULL;

	// Setup image/texture vertex data
	float left, right, top, bottom;
	float width, height;

	width = (float) new_image->width / (float) AGL_GetRenderingContext()->width;
	height = (float) new_image->height / (float) AGL_GetRenderingContext()->height;

	left = -(width / 2);
	right = -left;
	top = height / 2;
	bottom = -top;

	new_image->vertex[0].x = left;
	new_image->vertex[0].y = bottom;
	new_image->vertex[1].x = right;
	new_image->vertex[1].y = bottom;
	new_image->vertex[2].x = right;
	new_image->vertex[2].y = top;
	new_image->vertex[3].x = left;
	new_image->vertex[3].y = top;

	new_image->vertex[0].tx = 0;
	new_image->vertex[0].ty = (float) image_data->height;
	new_image->vertex[1].tx = (float) image_data->width;
	new_image->vertex[1].ty = (float) image_data->height;
	new_image->vertex[2].tx = (float) image_data->width;
	new_image->vertex[2].ty = 0;
	new_image->vertex[3].tx = 0;
	new_image->vertex[3].ty = 0;
	new_image->vertex_start = AGL_VertexGetVertexStart(new_image);

	glDisable(GL_TEXTURE_RECTANGLE_ARB);

	return new_image;
}

AGL_ImageData *SGL_LoadTextureFromFile(const char *filename)
{
	SDL_RWops *rwops;
	SDL_Surface *img_surface = NULL;
	AGL_ImageData *image_data = NULL;

	if (!AGL_GetRenderingContext())
		return NULL;

	if (filename)
	{
		fprintf(stderr, "Loading image %s\n", filename);
		image_data = (AGL_ImageData *) calloc(1, sizeof(AGL_ImageData));
		image_data->vertex = (AGL_Vertex *) calloc(1, sizeof(AGL_Vertex) * 4);

		// Use SDL_Image to open the image and read it in as an SDL_Surface
		rwops = SDL_RWFromFile(filename, "r");

		if (rwops)
			img_surface = IMG_Load_RW(rwops, 1);

		if (!img_surface)
		{
			image_data->width       = 0;
			image_data->height      = 0;
			image_data->texture_id  = -1;
			return image_data;
		}

		// Check to see if there is alpha (image transparency) in this image
		if (img_surface->format->Amask > 0)
			image_data->texture_flags |= AGL_IMG_ALPHA;
		else
			image_data->texture_flags &= ~AGL_IMG_ALPHA;

		// Continue generating texture from image data
		GLubyte *pixel_data = NULL;

		pixel_data = (GLubyte *) img_surface->pixels;

		glEnable(GL_TEXTURE_RECTANGLE_ARB);

		glGenTextures(1, &image_data->texture_id);
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, image_data->texture_id);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// SDL rows aligned to 4 byte boundary, we need to tell OpenGL about it
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB,
					 0,
					 (image_data->texture_flags & AGL_IMG_ALPHA) ? GL_RGBA : GL_RGB,
					 img_surface->w,
					 img_surface->h,
					 0,
					 (image_data->texture_flags & AGL_IMG_ALPHA) ? GL_RGBA : GL_RGB,
					 GL_UNSIGNED_BYTE,
					 pixel_data);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		image_data->width   = img_surface->w;
		image_data->height  = img_surface->h;
		image_data->bpp = (image_data->texture_flags & AGL_IMG_ALPHA) ? 4 : 3;
		image_data->alpha   = 0.0f;

		// Setup image/texture vertex data
		float left, right, top, bottom;
		float width, height;

		width   = (float) image_data->width / (float) AGL_GetRenderingContext()->width;
		height  = (float) image_data->height / (float) AGL_GetRenderingContext()->height;

		left    = -(width / 2);
		right   = -left;
		top     = height / 2;
		bottom  = -top;

		image_data->vertex[0].x = left;
		image_data->vertex[0].y = bottom;
		image_data->vertex[1].x = right;
		image_data->vertex[1].y = bottom;
		image_data->vertex[2].x = right;
		image_data->vertex[2].y = top;
		image_data->vertex[3].x = left;
		image_data->vertex[3].y = top;

		image_data->vertex[0].tx = 0;
		image_data->vertex[0].ty = (float) image_data->height;
		image_data->vertex[1].tx = (float) image_data->width;
		image_data->vertex[1].ty = (float) image_data->height;
		image_data->vertex[2].tx = (float) image_data->width;
		image_data->vertex[2].ty = 0;
		image_data->vertex[3].tx = 0;
		image_data->vertex[3].ty = 0;
		image_data->vertex_start = AGL_VertexGetVertexStart(image_data);

		glDisable(GL_TEXTURE_RECTANGLE_ARB);

		// Free original image surface data
		SDL_FreeSurface(img_surface);
	}

	return image_data;
}

void AGL_DeleteTexture(AGL_ImageData *image_data)
{
	if (!image_data)
		return;

	// Already must be removed
	if (image_data->texture_id == 0)
		return;

	glDeleteTextures(1, &image_data->texture_id);

	free(image_data->vertex);
	image_data->texture_generated = 0;
	image_data->texture_id = 0;
	image_data->texture_flags = 0;
	image_data->texture_format = 0;
	image_data->width = 0;
	image_data->height = 0;
	image_data->bpp = 0;
	image_data->vertex_start = 0;
}
