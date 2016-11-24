#version 140

uniform sampler2D texture;

in vec2 fragTextCoord;

void main(void) 
{
	/* The OpenGL convention (origin at the bottom-left corner) is different than in 
	2D applications (origin at the top-left corner). It is necessary to swap the 
	texture Y coordinates */
	vec2 flipped_texcoord = vec2(fragTextCoord.x, 1.0 - fragTextCoord.y);

	gl_FragColor = texture2D(texture, flipped_texcoord);
}