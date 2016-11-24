#version 140

uniform mat4 mvp;

in vec3 vertCoord;
in vec2 vertTextCoord;
			
out vec2 fragTextCoord;

void main(void) 
{
	gl_Position = mvp * vec4(vertCoord, 1.0);

	/* Forward information to the fragment shader */
	fragTextCoord = vertTextCoord;
}