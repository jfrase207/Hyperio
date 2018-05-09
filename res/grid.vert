#version 130

attribute vec3 position;
attribute vec2 texCoord;

uniform mat4 transform;


varying vec3 texcoords;



void main()
{
    gl_Position = transform * vec4(position.x,position.y,position.z, 1.0);




}

	