/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <string>
#include <vector>
#include <stdint.h>

#include <agl_common.h>
#include <agl_resource.h>

// The following are defines of what I would like to be possible if I extend this project
#define IMG_SMOOTH			0x0010
#define IMG_ALPHA			0x0100
#define IMG_REPEAT			0x0200
#define IMG_MIPMAPS			0x0400
#define IMG_RAW				0x0800
#define IMG_COMPRESS		0x1000
#define IMG_TEXRECT			0x2000
#define IMG_YCBCR			0x4000

/// <summary>
/// This struct defines the data for a raw asset resource to be used by this game
/// </summary>
typedef struct s_resource
{
	int32_t id;
	uint8_t rc_type;
	uint32_t format;
	uint32_t load_type;
	int32_t generated;
	std::string filename;
	void* data_ptr;
	void (*func_ptr)(struct s_resource*);

} sResource;

/// <summary>
/// This enum describes the different asset resource types. At this point I'm only
/// really implementing image support
/// </summary>
typedef enum e_resource_type
{
	RESOURCE_IMAGE	= 0,	// I'm really only implementing this
	RESOURCE_AUDIO,
	RESOURCE_VIDEO

} eResourceType;

/// <summary>
/// This class manages asset resources to be used by the game
/// </summary>
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void LoadResources(std::string asset_config_file);
	void DestroyResources();

	sResource* GetResource(int32_t index);
	AGL_ImageData* GetImageDataResource(int32_t index);

private:

	int readLine(FILE* f, char* line);
	int readWord(char** src, char* word);
	void loadTextures();

	std::vector<sResource*> main_resource_list;

};

#endif