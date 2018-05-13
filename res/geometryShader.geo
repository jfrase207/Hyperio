#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;;

uniform float time;
uniform float mag;



out vec3 normal;

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  

vec4 explode(vec4 position, vec3 normal)
{
    //float magnitude = 10.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * mag; 
    return position + vec4(direction, 0.0);
} 

void main()
{
normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
   
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
   
    EmitVertex();
    EndPrimitive();
}