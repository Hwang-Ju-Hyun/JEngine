#version 460

layout (location = 0) in vec3 pos;
layout (location=2) in vec3 aTexCoord;

uniform mat3 uModelToNDC;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(vec2(uModelToNDC * vec3(pos.xy, 1.0)), 0.0, 1.0);
	TexCoord=aTexCoord.xy;
}