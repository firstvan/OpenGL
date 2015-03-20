#version 400

in vec3 Position;
in vec3 Normal;

struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // A,D,S intensity
};
uniform LightInfo Light[5];

struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
};
uniform MaterialInfo Material;

vec3 ambAndDiff, spec;

layout( location = 0 ) out vec4 FragColor;

void phongModel(int i, vec3 pos, vec3 norm, out vec3 ambAndDiff, out vec3 spec ) {
    vec3 s = normalize(vec3(Light[i].Position) - pos);
    vec3 v = normalize(-pos.xyz);
    vec3 r = reflect( -s, norm );
    vec3 ambient = Light[i].Intensity * Material.Ka;
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = Light[i].Intensity * Material.Kd * sDotN;
    spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light[i].Intensity * Material.Ks *
               pow( max( dot(r,v), 0.0 ), Material.Shininess );
    ambAndDiff = ambient + diffuse;
}

void main() {
	vec4 color = vec4(0.0);
	
	for( int i = 0; i < 5; i++ )
	{
		vec3 ambAndDiff, spec;
		phongModel( i, Position, Normal, ambAndDiff, spec );
		color += (vec4( ambAndDiff, 1.0 )) + vec4(spec,1.0);
	}
	FragColor = color;
	//FragColor = (vec4( 1.0, 0.0, 0.0, 1.0 ));
}
