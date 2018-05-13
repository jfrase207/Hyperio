#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float distance;

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

normal = v_norm[0];

for(int i; i < 3; i++)
{

gl_Position = gl_in[i].gl_Position;

float dist = gl_Position.z - distance;

    gl_Position = gl_in[i].gl_Position;

	if(dist < 250)
       EmitVertex();

}

EndPrimitive();   
   

}