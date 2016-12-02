#version 330

uniform mat4 mvp;

in vec3 vertCoord;
in vec2 texCoord;

/*out float r;
out float g;
out float b;
out float a;*/

out vec2 fragTexCoord;

void main(void) 
{
	gl_Position = mvp * vec4(vertCoord, 1.0);
	fragTexCoord = texCoord;


	/* int offset = 9; // 0 = red, 3 = green, 6 = blue, 9 = alpha
	r = texelFetch(tex, offset + 0).r;
	g = texelFetch(tex, offset + 1).g;
	b = texelFetch(tex, offset + 2).b;
	a = texelFetch(tex, offset + 3).a; */ 
}