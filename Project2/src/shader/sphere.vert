#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main()
{
	TexCoords = VertexTexCoord;
	Normal = normalize( NormalMatrix * VertexNormal);
	FragPos = vec3( ModelViewMatrix * vec4(VertexPosition,1.0) );
	gl_Position = MVP * vec4(VertexPosition,1.0);
}