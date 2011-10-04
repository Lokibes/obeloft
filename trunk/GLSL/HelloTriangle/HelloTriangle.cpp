
#include <stdlib.h>
#include <stdio.h>

#include "esUtil.h"
#include "load_Shader.h"
#include "TextureLoader.h"
//#include "Texture.h"

extern bool DEBUG;

#define texSkyBack	"sarah_color.tga"

typedef struct
{
   // Handle to a program object
   GLuint programObject;
   GLuint iVerticesLoc;
   GLuint iColorLoc;
   GLuint iVerticesLoc1;
   GLuint iColorLoc1;
} UserData;

// Create a shader object, load the shader source, and compile the shader.
GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;
   
   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   	return 0;

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );
   
   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled ) 
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
         char* infoLog = (char*)malloc (sizeof(char) * infoLen );

         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         esLogMessage ( "Error compiling shader:\n%s\n", infoLog );            
         
         free ( infoLog );
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;

}

void LoadTexture ()	{
	//	Generating texture
	Texture* tex = loadTexture(texSkyBack, loadTGA);
	glGenTextures(1, &(tex->texID));

	//	Binding and loading
	glBindTexture(GL_TEXTURE_2D, tex->texID);
	glTexImage2D(GL_TEXTURE_2D, 0, tex->texType, tex->width/2, tex->height/2, 0, tex->texType, GL_UNSIGNED_BYTE, tex->imageData);

	printTexInfo(tex);

	//	Setting parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
}

// Initialize the shader and program object
int Init ( ESContext *esContext )	{
	UserData *userData = (UserData *)esContext->userData;
	char * vShaderStr = ReadFile("./vshader.glsl");
	char * fShaderStr = ReadFile("./fshader.glsl");

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	// Load the vertex - fragment shaders
	vertexShader = LoadShader ( GL_VERTEX_SHADER, (char*)vShaderStr );
	fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, (char*)fShaderStr );

	// Create the program object
	programObject = glCreateProgram ( );

	if ( programObject == 0 )
		return 0;

	glAttachShader ( programObject, vertexShader );
	glAttachShader ( programObject, fragmentShader );


	// Bind vPosition to attribute 0   
	// glBindAttribLocation ( programObject, iVerticesLoc, "a_position" );

	// Link the program
	glLinkProgram ( programObject );

	// Check the link status
	glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

	if ( !linked )	{
		GLint infoLen = 0;

		glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )	{
			char* infoLog = (char*)malloc (sizeof(char) * infoLen );

			glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
			esLogMessage ( "Error linking program:\n%s\n", infoLog );            

			free ( infoLog );
		}

		glDeleteProgram ( programObject );
		return FALSE;
	}

	// Store the program object
	userData->programObject = programObject;

	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
	return TRUE;
}

// Draw, using the shaders created in Init()
void Draw ( ESContext *esContext )	{
	UserData *userData = (UserData *)esContext->userData;

	GLfloat aColor_red[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f	};

	// Set the viewport
	glViewport ( 0, 0, esContext->width, esContext->height );

	// Use the program object
	glUseProgram ( userData->programObject );

	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	

	GLfloat myUnits_mat[] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f	};

	//	position of the triangle
	GLfloat vVertices[] = {  
		 0.0f,  0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		 0.5f, -0.5f, 0.5f };

	GLfloat texcoords_pos[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f
	}; 

	//	Draw the triangle
	//	Get attributes, uniform locations
	GLuint matLocation = glGetUniformLocation(userData->programObject, "u_mvpMatrix");
	userData->iVerticesLoc = glGetAttribLocation ( userData->programObject , "a_position" );
	//userData->iColorLoc = glGetAttribLocation ( userData->programObject , "a_color" );	//	for using color
	userData->iColorLoc = glGetAttribLocation ( userData->programObject , "a_uv" );			//	for using texture

	//	Update their value stored in "program" through our initialized variable
	glUniformMatrix4fv ( matLocation, 1, GL_FALSE, myUnits_mat );
	glVertexAttribPointer ( userData->iVerticesLoc, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
	//glVertexAttribPointer ( userData->iColorLoc, 4, GL_FLOAT, GL_FALSE, 0, aColor_red );
	glVertexAttribPointer ( userData->iColorLoc, 4, GL_FLOAT, GL_FALSE, 0, texcoords_pos );

	//	Enable some shit things in order to make it work =.=
	glEnableVertexAttribArray ( glGetUniformLocation(userData->programObject, "u_mvpMatrix") );
	glEnableVertexAttribArray ( userData->iVerticesLoc );
	glEnableVertexAttribArray ( userData->iColorLoc );

	//	Finally, draw as crazily as we can
	glDrawArrays ( GL_TRIANGLES, 0, 3 );

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

int main ( int argc, char *argv[] )	{
	ESContext esContext;
	UserData  userData;

	esInitContext ( &esContext );
	esContext.userData = &userData;

	esCreateWindow ( &esContext, "Hello Triangle", 800, 600, ES_WINDOW_RGB );

	if ( !Init ( &esContext ) )
	return 0;

	LoadTexture();

	esRegisterDrawFunc ( &esContext, Draw );

	esMainLoop ( &esContext );

	return 1;
}
