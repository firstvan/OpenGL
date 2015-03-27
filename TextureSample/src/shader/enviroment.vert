#version 430
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 TextureCord;
out vec3 Position;
out vec3 Normal;
out vec3 ReflectDir;

uniform mat4 MVP;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;


void main()
{
	
	ReflectDir = VertexPosition;

	Normal = normalize(NormalMatrix * VertexNormal);
	Position = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));
	
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}
