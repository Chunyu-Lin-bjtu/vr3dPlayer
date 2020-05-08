#version 330

in vec2 out_uv;
uniform sampler2D texture;
out vec4 frag_color;

void main()
{
	frag_color = texture2D (texture, out_uv);
  
	//vec3 result = texture2D(texture, vec2(out_uv.x, 1.0 - out_uv.y)).rgb;
	//frag_color	= vec4(result, 1.0f);
}
