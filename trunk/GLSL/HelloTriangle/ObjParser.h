
#ifndef __OBJ_PARSER__
#define __OBJ_PARSER__

typedef struct	{
	GLfloat vertices[];
	GLfloat textcoords[];
	GLfloat normalvectors[];
} Obj3D;

Obj3D* parse(char* objname);

#endif