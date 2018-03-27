#version 450

//uniform mat4 u_pm;
//uniform mat4 u_vm;
uniform vec3 lightDir;
uniform float minDist;
uniform float maxDist;
uniform vec3 fogColor;
uniform vec3 zposplayer;
uniform vec3 zposasteroid;
uniform vec3 _color;
layout( location = 0 ) out vec4 fragcolor;

float gl_CullDistance[];
 
in vec3 v_norm;
in vec4 v_pos; 
 
vec3 toonRim() {
 
	/* vec3 n = normalize(mat3(u_vm) * normalize(v_norm));      // convert normal to view space
	vec3 p = vec3((u_pm) * v_pos);                // position in clip space
	vec3 v = normalize(p);                        // normalised eye vector
	float vdn = 0.6 - max(dot(v, n), 0.0);        // the rim contribution
	*/
	
	float intensity;
	vec4 color;
	intensity = dot(lightDir,normalize(v_norm));

	if (intensity > 0.95)
		color = vec4((_color.r * 0.8),(_color.g * 0.8),(_color.b * 0.8),1.0);
	else if (intensity > 0.5)
		color = vec4((_color.r * 0.5),(_color.g * 0.5),(_color.b * 0.5),1.0);
	else if (intensity > 0.25)
		color = vec4((_color.r * 0.3),(_color.g * 0.3),(_color.b * 0.3),1.0);
	else
		color = vec4((_color.r * 0.1),(_color.g * 0.1),(_color.b * 0.1),1.0);


  fragcolor.a = 0.6;
  //vec3(smoothstep(0.4, 0.8, vdn)) + 
  return vec3(color.x,color.y,color.z);
}

void main() {

	float dist = sqrt((zposplayer.x-zposasteroid.x) * (zposplayer.x-zposasteroid.x) + (zposplayer.y-zposasteroid.y) * (zposplayer.y-zposasteroid.y) + (zposplayer.z-zposasteroid.z) * (zposplayer.z-zposasteroid.z));
	
	gl_CullDistance[v_pos] = 1;

	//float dist = abs( zpos );

	float fogFactor = (maxDist - dist) / (maxDist - minDist);
	fogFactor = clamp( fogFactor, 0.0, 1.0 );
	vec3 toonRim = toonRim(); // this is your method from above

	vec3 color = mix( fogColor, toonRim, fogFactor);
	fragcolor = vec4(color, 1.0);

}
