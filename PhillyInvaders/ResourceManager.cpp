/************************************************************************************************************
* PhillyInvaders Game																						*
*																											*
* A simple space invaders like game which makes use of the AvaacGL graphics renderer						*
*																											*
* Author: Phillip Haddad																					*
*																											*
* ***********************************************************************************************************/

#include <stdint.h>
#include <iostream>
//#include <stdio.h>
#include <string>
#include <vector>

#include "ResourceManager.h"

#define READ_STATE_ID		0
#define READ_STATE_FILE		1
#define RC_TYPE_ID_MAX		5

static int resource_type;

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

// I love basic low-level C file handling, it's a hard thing to change and use modern streams when an insane
// amount of developer hours have gone into what made modern stream APIs possible in the first place
void ResourceManager::LoadResources(std::string asset_config_file)
{
	FILE* config_file;
	char line[1024], word[1024];
	char* c;
	int len = 0, index = 0, state = READ_STATE_ID;

	if (!asset_config_file.empty())
	{
		config_file = fopen(asset_config_file.c_str(), "r");
		if (!config_file)
		{
			std::cout << "[ERROR]: Could not load file " << asset_config_file << std::endl;
			return;
		}

		std::cout << "[INFO]: Loaded asset config file " << asset_config_file << std::endl;

		while (!feof(config_file))
		{
			if (readLine(config_file, line))
			{
				c = line;
				if (*c != '#')
				{
					while (readWord(&c, word))
					{
						len = strlen(word);
						if (len != 0)
						{
							switch (state)
							{
							case READ_STATE_ID:
								state = READ_STATE_FILE;

								char rc_type[RC_TYPE_ID_MAX + 1];
								for (int k = 0; k < RC_TYPE_ID_MAX; k++)
								{
									rc_type[k] = word[k];
								}
								rc_type[RC_TYPE_ID_MAX] = '\0';

								// Only really using image type here but might as well stick it in
								if (!strcmp("IMAGE", rc_type))
									resource_type = RESOURCE_IMAGE;
								if (!strcmp("AUDIO", rc_type))
									resource_type = RESOURCE_AUDIO;
								if (!strcmp("VIDEO", rc_type))
									resource_type = RESOURCE_VIDEO;
								break;
							case READ_STATE_FILE:
								if (resource_type == RESOURCE_IMAGE)
								{
									sResource *new_resource = (sResource *) calloc(1, sizeof(sResource));
									if (new_resource)
									{
										new_resource->filename = word;
										new_resource->data_ptr = AGL_LoadImageDataFromFile(word);
										new_resource->id = main_resource_list.size() + 1;
										new_resource->rc_type = RESOURCE_IMAGE;
										main_resource_list.push_back(new_resource);
									}
								}
								break;
							default:
								std::cout << "[ERROR]: Uhhhh, why did we come here?!?!" << std::endl;
								break;
							}
						}

						if (*c == '\n')
						{
							state = READ_STATE_ID;
							break;
						}
						c++;	// Move on to next char, I know this isn't efficient by todays standards but showing off lower-level memory saving code
					}
				}
			}
		}

		fclose(config_file);
	}

	loadTextures();
}

void ResourceManager::DestroyResources()
{
	for (size_t i = 0; i < main_resource_list.size(); ++i)
	{
		if (main_resource_list[i]->data_ptr)
			free(main_resource_list[i]->data_ptr);

		free(main_resource_list[i]);
	}

	main_resource_list.clear();
}

sResource* ResourceManager::GetResource(int32_t index)
{
	if (index >= 0 && index < main_resource_list.size())
		return main_resource_list[index];
	return NULL;
}

AGL_ImageData* ResourceManager::GetImageDataResource(int32_t index)
{
	if (index >= 0 && index < main_resource_list.size())
		return (AGL_ImageData*)GetResource(index)->data_ptr;
	return NULL;
}

int ResourceManager::readLine(FILE* f, char* line)
{
	return (fgets(line, 1023, f) != NULL);
}

int ResourceManager::readWord(char** src, char* word)
{
	char* c;
	c = *src;

	while (*c != ' ' && *c != '\n' && *c != ',' && *c != '\t')
		*word++ = *c++;
	*word = 0;
	*src = c;

	return 1;
}

void ResourceManager::loadTextures()
{
	if (main_resource_list.size() > 0)
	{
		for (size_t i = 0; i < main_resource_list.size(); ++i)
		{
			if (main_resource_list[i]->data_ptr != NULL)
			{
				if (main_resource_list[i]->rc_type == RESOURCE_IMAGE)
				{
					AGL_ImageData* img_data = (AGL_ImageData*)main_resource_list[i]->data_ptr;
					if (img_data->texture_id == 0)
					{
						void* old_ptr = 0, * new_ptr = 0;
						old_ptr = main_resource_list[i]->data_ptr;
						new_ptr = (void*)AGL_LoadTextureFromImage(img_data);
						if (new_ptr)
						{
							main_resource_list[i]->data_ptr = new_ptr;
							free(old_ptr);
						}
					}
				}
			}
		}
	}
}