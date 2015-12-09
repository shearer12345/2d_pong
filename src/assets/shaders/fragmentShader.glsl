#version 330
in vec4 fragmentColor;
in vec2 fragmentUV;
out vec4 outputColor;
uniform sampler2D textureSampler;

void main()
{
	 outputColor = fragmentColor * texture(textureSampler, fragmentUV);
}
