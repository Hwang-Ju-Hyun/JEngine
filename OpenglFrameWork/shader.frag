#version 460

out vec4 color;
in vec2 TexCoord;

uniform vec4 uOutColor;          // ����
uniform sampler2D ourTexture;    // �ؽ�ó
uniform bool hasTexture;         // �ؽ�ó�� �ִ��� ���θ� ��Ÿ���� ����

void main()
{
    if (hasTexture) // �ؽ�ó�� �ִ� ���
    {
        color = texture(ourTexture, TexCoord);
    }
    else // �ؽ�ó�� ���� ���
    {
        color = uOutColor; // RGB ���� ���
    }
}