#version 430

in vec3 Position;
in vec3 Normal;

//struct LightInfo {
//	vec4 Position;
//	vec3 Intensity;
//};
//uniform LightInfo Light[2];

struct SpotLightInfo {
 vec4 Position;
 vec3 intensity;
 vec3 direction;
 float exponent;
 float cutoff;
};

uniform SpotLightInfo sli;

struct MaterialInfo{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
};
uniform MaterialInfo Material;


layout (location=0) out vec4 FragColor;

/*void phongModel(vec3 pos, vec3 norm, out vec3 ambAndDiff, out vec3 spec)
{
	vec3 s = normalize(vec3(sli.Position)-pos);
	vec3 v = normalize(-pos.xyz);
	vec3 r = reflect(-s, norm);
	vec3 ambient = Light[i].Intensity * Material.Ka;
	float sDotN = max(dot(s, norm), 0.0);
	vec3 diffuse = Light[i].Intensity * Material.Kd * sDotN;
	spec = vec3(0.0);
	if(sDotN> 0.0)
		spec = Light[i].Intensity * Material.Ks * pow(max(dot(r,v), 0.0), Material.Shininess);
		ambAndDiff = ambient+diffuse;
}*/

vec3 adsWithSpotLight()
{
	vec3 s = normalize(vec3(sli.Position) - Position);
	float angle = acos( dot(-s, sli.direction));
	float cutoff = radians(clamp(sli.cutoff, 0.0, 90.0));
	vec3 ambient = sli.intensity * Material.Ka;

	if( angle < cutoff ) {
		float spotFactor = pow( dot(-s, sli.direction), sli.exponent );
		vec3 v = normalize(vec3(-Position));
		vec3 h = normalize( v + s );
		return ambient + spotFactor * sli.intensity * ( Material.Kd * max( dot(s, Normal), 0.0 ) +
			Material.Ks * pow(max(dot(h,Normal), 0.0),Material.Shininess));
	} else {
		return ambient;
	}
}

void main() {
	vec4 color =vec4(0.0);
	//for(int i = 0; i < 2; i++)
	//{
		//vec3 ambAndDiff, spec;
		//phongModel(i, Position, Normal, ambAndDiff, spec);
		//color += vec4(ambAndDiff, 1.0)+vec4(spec, 1.0);
	//}

    FragColor = vec4(adsWithSpotLight(), 1.0);

}
