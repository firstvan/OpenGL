#version 430

layout (location=0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform mat4 MVP;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

out vec3 Position;
out vec3 Normal;


void main()
{
	Normal = normalize(NormalMatrix * VertexNormal);
	Position = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
