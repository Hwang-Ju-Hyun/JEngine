#version 460

out vec4 color;
in vec2 TexCoord;

uniform vec4 uOutColor;          
uniform sampler2D uOutTexture;    
uniform bool uHasTexture;         

void main()
{
    if (uHasTexture) 
    {
        color = texture(uOutTexture, TexCoord);
    }
    else 
    {
        color = uOutColor;
    }
}