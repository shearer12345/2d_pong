#version 450
in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;
out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 modelMatrix      = mat4(1.0);
uniform mat4 viewMatrix       = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

void main()
{
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
		fragmentColor = vertexColor;
		fragmentUV = vertexUV;
}
