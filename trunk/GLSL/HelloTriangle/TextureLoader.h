
#ifndef __TEX_LOADER__
#define __TEX_LOADER__

#include "esUtil.h"

typedef struct {
	GLuint	texID;			// Texture gened ID
	GLuint	texType;		// Image Type (GL_RGB, GL_RGBA)
	GLuint	bpp;			// Image Color Depth In Bits Per Pixel
	GLuint	width;			// Image Width
	GLuint	height;			// Image Height
	GLubyte	* imageData;	// Image Data (Up To 32 Bits)
} Texture;

// This will call necessary function for each file-type
// So we can extend our texture-loader later
Texture* loadTexture(char* filename, Texture* (*loadFileType) (char*));

// Considered as our "extensions", each can loads a specific file-type
Texture* loadTGA(char* tgaName);
Texture* loadBMP(char* bmpName);
Texture* loadJPG(char* jpgName);

//	Print info and compare to result of some image viewer to check
void printTexInfo(Texture* texture);

#endif