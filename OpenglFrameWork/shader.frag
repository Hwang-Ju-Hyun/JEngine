#version 460

out vec4 color;
in vec2 TexCoord;

uniform vec4 uOutColor;          // 색상
uniform sampler2D ourTexture;    // 텍스처
uniform bool hasTexture;         // 텍스처가 있는지 여부를 나타내는 변수

void main()
{
    if (hasTexture) // 텍스처가 있는 경우
    {
        color = texture(ourTexture, TexCoord);
    }
    else // 텍스처가 없는 경우
    {
        color = uOutColor; // RGB 색상 사용
    }
}