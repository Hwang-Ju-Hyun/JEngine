#version 460

layout (location = 0) in vec2 pos;

uniform mat3 uModelToNDC;

void main()
{
	gl_Position = vec4(vec2(uModelToNDC * vec3(pos, 1.0)), 0.0, 1.0);
}