#version 140

uniform samplerCube skybox;

in vec3 fragTextCoord;

void main()
{
	gl_FragColor = texture(skybox, fragTextCoord);
}