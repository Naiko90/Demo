#version 140

uniform mat4 matrix;

in vec3 vertCoord;

out vec3 fragTextCoord;

/* A cubemap used to texture a 3D cube can be sampled using the positions of the 
cube as the texture coordinates. When a cube is centered on the origin (0,0,0) 
each of its position vectors is also a direction vector from the origin. 
This direction vector is exactly what we need to get the corresponding texture 
value at that specific cube's position. For this reason, we only need to supply 
position vectors and don't need texture coordinates. */

void main()
{
	vec4 pos = matrix * vec4(vertCoord, 1.0);
	gl_Position = pos.xyww;
	fragTextCoord = vertCoord;
}