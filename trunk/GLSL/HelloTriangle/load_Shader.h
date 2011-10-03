char * ReadFile(char *fileName)
{
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;

	pFile = fopen (fileName, "r" );
	if (pFile==NULL) 
	{
		fputs ("File error",stderr); exit (1);
	}

	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	buffer = (char*) malloc (sizeof(char)*lSize + 1);
	memset(buffer, 0, sizeof(char)*lSize + 1);

	if (buffer == NULL) 
	{
		fputs ("Memory error",stderr); exit (2);
	}

	result = fread (buffer,1,lSize,pFile);
	buffer[sizeof(char)*lSize] = '\0';

	//printf("readfile: %s \n", buffer);
	fclose (pFile);
	return buffer;
}


#define BITMAP_ID 0x4D42                // the universal bitmap ID

BITMAPINFOHEADER	bitmapInfoHeader;	// bitmap info header
unsigned char* szBuf;
static int iImgW = 0;
static int iImgH = 0;

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;								// the file pointer
	BITMAPFILEHEADER	bitmapFileHeader;		// bitmap file header
	unsigned char		*bitmapImage;			// bitmap image data
	int					imageIdx = 0;			// image index counter
	unsigned char		tempRGB;				// swap variable

	// open filename in "read binary" mode
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	// verify that this is a bitmap by checking for the universal bitmap id
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// read the bitmap information header
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// move file pointer to beginning of bitmap data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// allocate enough memory for the bitmap image data
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// verify memory allocation
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	iImgH = bitmapInfoHeader->biHeight;
	iImgW = bitmapInfoHeader->biWidth;

	// make sure bitmap image data was read
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	
		
	// close the file and return the bitmap image data
	fclose(filePtr);
	return bitmapImage;
}

