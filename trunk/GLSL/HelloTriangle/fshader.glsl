#include <stdio>
#include <time.h>

precision lowp float;
varying vec4 v_color;

void main()
{
	/* initialize random seed: */
	srand ( time(NULL) );

	/* generate secret number: */
	float red = (rand() % 10) * 0.1;
	float green = (rand() % 10) * 0.1;
	float blue = (rand() % 10) * 0.1;

	//gl_FragColor = v_color ;              
	gl_FragColor =  vec4 ( red, green, blue, 1.0 );
} 
