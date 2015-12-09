#version 330
in vec4 fragmentColor;
in vec2 fragmentUV;
out vec4 outputColor;
void main()
{
	 outputColor = fragmentColor * vec4(0.0, fragmentUV, 1.0) ;
}
