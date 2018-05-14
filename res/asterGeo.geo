#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform vec3 playerPos;
uniform float cullDistance;

in vec3 v_norm[];
out vec3 normal;

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  


void main()
{

	normal = v_norm[2];
	float dist;

	for(int i; i < 3; i++)
	{

		gl_Position = gl_in[i].gl_Position;
		
		dist = sqrt((playerPos.x-gl_in[i].gl_Position.x) * (playerPos.x-gl_in[i].gl_Position.x) + (playerPos.y-gl_in[i].gl_Position.y) * (playerPos.y-gl_in[i].gl_Position.y) + (playerPos.z-gl_in[i].gl_Position.z) * (playerPos.z-gl_in[i].gl_Position.z));			

		if(dist < cullDistance)
			EmitVertex();

	}

	
		EndPrimitive();   
   

}