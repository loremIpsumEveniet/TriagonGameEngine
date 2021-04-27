#pragma once
#include "dependencies.h"

///STB Image Loader/// 
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h> //For some reason this can't go into dependencies

unsigned int LoadTexture(const char* Path, bool ImageFormatHasAlphaData) {

	unsigned int texture;
	glGenTextures(1, &texture); //the the texture variable an ID within the GPU
	glBindTexture(GL_TEXTURE_2D, texture); //"Bind" the variable to a "slot" in/at the GPU

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int Width, Height, NumOfColorChannels;
	unsigned char* ImageColorData = stbi_load(Path, &Width, &Height, &NumOfColorChannels, 0);

	if (ImageColorData) {
		if (ImageFormatHasAlphaData) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageColorData);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, ImageColorData);
		}
		/*
		glTexImage2D(
		GPU "slot" type,
		Mipmap level,
		How to save the texture,
		Width,
		Height,
		Legacy Always 0,
		What type of data we stored in ImageColorData
		How we stored the data in ImageColorData
		The actual variable with the color data)
		*/
		stbi_image_free(ImageColorData);
	}
	else {
		printf("Could Not Load Texture\n");
		return NULL;
	}
	return texture;
}
