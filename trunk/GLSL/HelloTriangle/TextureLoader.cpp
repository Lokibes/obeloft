
//	Reference is at http://en.wikipedia.org/wiki/Truevision_TGA
//	And http://nehe.gamedev.net/tutorial/loading_compressed_and_uncompressed_tga%27s/22001/

#include <Windows.h>
#include <stdio.h>

#include "TextureLoader.h"

bool DEBUG = true;

void printTexInfo(Texture* texture)	{
	printf("Bits per pixel\t\t%i\n", texture->bpp);
	printf("Width         \t\t%i\n", texture->width);
	printf("Height        \t\t%i\n", texture->height);
	printf("Type          \t\t%x\n", texture->texType);
}

Texture* loadTexture(char* filename, Texture* (*loadFileType) (char*))	{
	if (DEBUG)	{
		if (!filename)	{
			printf("MISSING file name !\n");
			return NULL;
		}
	}

	return loadFileType(filename);
}

Texture* loadTGA(char* tgaName)	{
	FILE* file = NULL;

	Texture* tex = new Texture;
	tex->texID = 0;
	tex->bpp = 0;
	tex->height = 0;
	tex->texType = 0;
	tex->width = 0;
	tex->imageData = NULL;

	file = fopen(tgaName, "r");

	if (DEBUG)	{
		if (!file)	{
			printf("OPEN file failed !\n");
			return NULL;
		}
	}

	//	obtain file size
	fseek (file , 0 , SEEK_END);
	long fsize = ftell (file);
	rewind (file);

	//	copy the file into the buffer
	byte* buffer = (byte*) malloc ( fsize * sizeof(byte) );
	size_t result = fread (buffer, 1, fsize, file);

	//	read length of skipped fields
	//	higher byte * 256 + lower byte (little-endian)
	GLuint colorMapSize = buffer[6] * 256 + buffer[5];
	GLuint skipSize = buffer[0] + colorMapSize;

	//	read some necessary fields and store in our struct
	tex->width = buffer[13] * 256 + buffer[12];
	tex->height = buffer[15] * 256 + buffer[14];
	tex->bpp = (GLuint)buffer[16];

	//	read the core data
	//	!NOTE : with the compressed TGA, size is not simply w x h :D
	tex->imageData = new GLubyte[tex->width * tex->height];
	for (int i = 17 + skipSize; i < 17 + skipSize + tex->width*tex->height; i ++)	{
		tex->imageData[i - 17 - skipSize] = buffer[i];
	}

	if(tex->bpp == 24)	tex->texType = GL_RGB;
	else				tex->texType = GL_RGBA;

	//	From NeHe : TGA files store their image in reverse order than what OpenGL wants
	//	So we must change the format from BGR to RGB. To do this we swap the first and third bytes in every pixel.
	for(GLuint i = 0; i < (int)tex->height * tex->width; i += tex->bpp)	{
        // 1st Byte XOR 3rd Byte XOR 1st Byte XOR 3rd Byte
        tex->imageData[i] ^= tex->imageData[i+2] ^=
        tex->imageData[i] ^= tex->imageData[i+2];
    }

	fclose(file);

	if (DEBUG)	{
		if (!tex)	{
			printf("CREATE texture failed !\n");
			return NULL;
		}
	}

	return tex;
}

Texture* loadBMP(char* bmpName);
Texture* loadJPG(char* jpgName);
