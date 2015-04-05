#version 430

in vec3 Position;
in vec3 Normal;
in vec2 TexCoords;

layout(binding = 0) uniform sampler2D Tex1;

layout (location = 0) out vec4 FragColor;


void main()
{	
	vec4 cubeMapColor = texture(Tex1, TexCoords);
	FragColor = cubeMapColor;
}