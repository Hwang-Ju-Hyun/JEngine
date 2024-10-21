#version 460

out vec4 color;
in vec2 TexCoord;


uniform vec4 uOutColor;
uniform sampler2D ourTexture;

void main()
{	
	color=texture(ourTexture,TexCoord);
}