#version 330

uniform float gScale;
uniform mat4 gWorld;

layout (location = 0) in vec3 Position;

out vec4 Color;

void main()
{

	gl_Position = gWorld * vec4(Position, 1.0);
	//gl_Position = vec4(gScale * Position.x, gScale * Position.y, Position.z, 1.0);
	Color = vec4((Position.x+1.0)/2,(Position.y+1.0)/2,(Position.z+1.0)/2, 1.0);
	//Color = vec4(1.0);
}