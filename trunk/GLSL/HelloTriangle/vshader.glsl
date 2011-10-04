uniform mat4 u_mvpMatrix;
attribute vec4 a_position;

attribute vec2 a_uv;
varying vec2 v_uv;
    
attribute vec4 a_color;	
varying vec4 v_color;

void main()	{
	//gl_Position = a_position;
	gl_Position = u_mvpMatrix * a_position;
	//v_color = a_color;		//	For using color
	v_uv = a_uv;				//	For using texture
}

