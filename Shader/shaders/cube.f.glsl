#version 330

uniform sampler2D tex;

in vec2 fragTexCoord;

/* in float r;
in float g;
in float b;
in float a; */

void main(void) 
{
	vec2 flipped_texcoord = vec2(fragTexCoord.x, 1.0 - fragTexCoord.y);
	gl_FragColor = texture2D(tex, flipped_texcoord);

	// gl_FragColor = vec4(r, g, b, a);
}