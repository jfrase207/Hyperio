#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform vec3 playerPos;
uniform vec3 zposrock;
uniform float cullDistance;

in vec3 v_norm[];
out vec3 normal;

//calculate face normals
vec3 GetNormal()
{
	vec3 tangent1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 tangent2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 normal = cross(tangent1, tangent2);
		return normalize(normal);
}  


void main()
{

	normal = GetNormal();	

	for(int i; i < 3; i++)
	{		
		float dist = sqrt((playerPos.x-zposrock.x) * (playerPos.x-zposrock.x) + (playerPos.y-zposrock.y) * (playerPos.y-zposrock.y) + (playerPos.z-zposrock.z) * (playerPos.z-zposrock.z));			
		gl_Position = gl_in[i].gl_Position;
		if(dist < cullDistance)
			EmitVertex();
	}	
	
	EndPrimitive();   
}