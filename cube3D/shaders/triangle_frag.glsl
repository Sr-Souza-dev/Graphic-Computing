#version 330 core

in vec3 Color;
out vec4 OutColor;

void main(){
	vec3 FinalColor = Color;
	OutColor = vec4(FinalColor, 1);
}