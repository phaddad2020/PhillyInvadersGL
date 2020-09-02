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

// Static globals
static int32_t context_created;
static AGL_RenderingContext *render_context;
static AGL_OpenGLInfo opengl_info;

// Static functions
static void agl_reshape(int32_t width, int32_t height, int32_t render_flags)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-AGL_VIEW_X,
			 AGL_VIEW_X,
			-AGL_VIEW_Y,
			 AGL_VIEW_Y,
			 AGL_VIEW_Z_NEAR,
			 AGL_VIEW_Z_FAR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void agl_set_2D_render_optimal_states(int32_t render_flags)
{
	// Set optimal states for 2D rendering in OpenGL
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_ALPHA_TEST);
	glShadeModel(GL_FLAT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
}

// Public Functions

AGL_RenderingContext * AGL_CreateRenderingContext(int32_t render_flags, int32_t width, int32_t height,
														const char *title)
{
	// Check first, at the current time you cannot create more than 1 rendering context
	// this will most likely change in future
	if (context_created)
		return NULL;

	// Initialise SDL with video only. SDL_INIT_NOPARACHUTE allows SDL to catch signals
	// where the app may seg fault allowing SDL to still clean up after itself
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0)
	{
		fprintf(stderr, "SDL Video Initialisation failed (%s).\n", SDL_GetError());
	}
	else
	{
		fprintf(stderr, "SDL Video Initialised successfully\n");

		// Create and initialise SMGL rendering context
		render_context = (AGL_RenderingContext *) calloc(1, sizeof(AGL_RenderingContext));

		if (!render_context)
		{
			fprintf(stderr, "SHFLGL Rendering Context could not be created, out of memory\n");
			return NULL;
		}
		else
		{
			// Setup double buffering
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			// Set up SDL Flags
			render_context->sdlFlags = SDL_OPENGL | SDL_RESIZABLE;

			if (render_flags & AGL_ENABLE_FULLSCREEN)
				render_context->sdlFlags |= SDL_FULLSCREEN;

			render_context->width = width;
			render_context->height = height;
			render_context->perf_stats = 0;

			// Set SDL Video Mode and create SDL Surface
			render_context->render_surface = SDL_SetVideoMode(render_context->width,
															  render_context->height,
															  24,
															  render_context->sdlFlags);


			// Initialise glew32 just after OpenGL context is created but before we start using OpenGL
			// to initialise anything else
			if (GLEW_OK != glewInit())
			{
				// GLEW failed!
				fprintf(stderr, "GLEW FAILED\n");
				exit(1);
			}

			if (!render_context->render_surface)
			{
				fprintf(stderr, "SDL Video Mode Failed (%s)\n", SDL_GetError());
				free(render_context);
				return NULL;
			}

			// Resize can be used to setup initial screen context
			agl_reshape(render_context->width, render_context->height, render_context->renderFlags);

			fprintf(stderr, "SHFLGL Rendering Context successfully created.\n");

			agl_set_2D_render_optimal_states(render_context->renderFlags);

			if (title)
			{
				#define BUF_SIZE 256
				char buf[BUF_SIZE];
				_snprintf(buf, BUF_SIZE - 1, "%s", title);
				SDL_WM_SetCaption(buf, NULL);
				#undef BUF_SIZE
			}

			context_created = 1;
		}
	}

	// Query OpenGL Info
	AGL_QueryOpenGLInfo(&opengl_info);

	return render_context;
}

AGL_RenderingContext *AGL_GetRenderingContext()
{
	return render_context;
}

AGL_OpenGLInfo *AGL_GetOpenGLInfo()
{
	return &opengl_info;
}

void AGL_QueryOpenGLInfo(AGL_OpenGLInfo *opengl_info)
{
	if (opengl_info)
	{
		opengl_info->renderer = glGetString(GL_RENDERER);
		opengl_info->vendor = glGetString(GL_VENDOR);
		opengl_info->version = glGetString(GL_VERSION);
		opengl_info->glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
		glGetIntegerv(GL_MAJOR_VERSION, &opengl_info->glslMajor);
		glGetIntegerv(GL_MINOR_VERSION, &opengl_info->glslMinor);

		// Print out queried information
		fprintf(stderr, "GL Vendor		: %s\n", opengl_info->vendor);
		fprintf(stderr, "GL Renderer	: %s\n", opengl_info->renderer);
		fprintf(stderr, "GL Version		: %s, (%d.%d)\n", opengl_info->version, opengl_info->glslMajor, opengl_info->glslMinor);
		fprintf(stderr, "GLSL Version	: %s\n", opengl_info->glslVersion);
	}
}

int agl_ErrorCheck(const char *GLcall, const char *file, int line)
{
	GLenum glErr;
	int retCode = 0;
	char const *enum_name = NULL;
	int err_seq = 0;
	while ((glErr = glGetError()) != GL_NO_ERROR)
	{
		switch (glErr)
		{
		case GL_INVALID_ENUM:		enum_name = "GL_INVALID_ENUM";		break;
		case GL_INVALID_VALUE:		enum_name = "GL_INVALID_VALUE";		break;
		case GL_INVALID_OPERATION:	enum_name = "GL_INVALID_OPERATION";	break;
		case GL_STACK_OVERFLOW:		enum_name = "GL_STACK_OVERFLOW";	break;
		case GL_STACK_UNDERFLOW:	enum_name = "GL_STACK_UNDERFLOW";	break;
		case GL_OUT_OF_MEMORY:		enum_name = "GL_OUT_OF_MEMORY";		break;
		default:					enum_name = "Unknown GL Error";		break;
		}
		fprintf(stderr, "[%d] %s: %s [%s:%d]\n", err_seq, GLcall, enum_name, file, line);
		++err_seq;
	}
	return retCode;
}