#version 430

in vec3 Position;
in vec3 Normal;
//in vec2 TextCoord;
in vec3 ReflectDir;

//layout(binding = 0) uniform sampler2D Text1;
layout(binding = 0) uniform samplerCube CubeMapTex;


/*struct LightInfo
{
	vec4 Position;
	vec3 Intensity;
};

uniform LightInfo light;

struct MaterialInfo
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
};

uniform MaterialInfo material;*/

layout (location = 0) out vec4 FragColor;

/*void phongModel(vec3 pos, vec3 norm, out vec3 ambDiff, out vec3 spec)
{
	vec3 s = normalize(vec3(light.Position) - pos);
	vec3 v = normalize(-pos.xyz);
	vec3 r = reflect(-s, norm);
	vec3 ambient = light.Intensity * material.Ka;
	float sDotN = max(dot(s, norm), 0.0);
	vec3 diffuse = light.Intensity * material.Ks * sDotN;
	spec = vec3(0.0);
	if(sDotN>0.0){
		spec = light.Intensity * material.Ks * pow(max(dot(r,v), 0.0), material.Shininess);
		ambDiff = ambient + diffuse; 
	}
}*/

void main()
{	
	/*vec3 ambdiff, spec;
	vec4 textColor = texture(Text1, TextCoord);
	phongModel(Position, Normal, ambdiff, spec);
	FragColor = vec4(adiff,1.0) * textColor + vec4(spec, 1.0);*/
	
	vec4 cubeMapColor = texture(CubeMapTex,ReflectDir);
	FragColor = cubeMapColor;
}