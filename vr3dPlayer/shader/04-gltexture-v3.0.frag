#version 330

in vec3 VertColor;
in vec2 TextCoord;

uniform sampler2D tex1;

out vec4 color;


void main()
{
	color = texture(tex1, vec2(TextCoord.s, 1.0 - TextCoord.t)); //翻转y轴
}