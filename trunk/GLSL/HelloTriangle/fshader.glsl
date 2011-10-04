precision lowp float;
varying vec4 v_color;

uniform sampler2D u_texture;
varying vec2 v_uv;

void main()	{
	//gl_FragColor = v_color;						//	For using color
	gl_FragColor = texture2D(u_texture, v_uv);		//	For using texture
} 
