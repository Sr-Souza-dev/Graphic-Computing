#version 330 core

in vec3 Color;
// Declarando uma vari�vel de sa�da do tipo vec4
out vec4 OutColor;

void main(){

	// Mesclando a Cor RGB informada com a textura,
	// Note que � uma multiplica��o de matrizes
	vec3 FinalColor = Color;
	OutColor = vec4(FinalColor, 1.0);

	//Para inverter a cor
	//vec3 FinalColor = -1.0 * Color * TextureColor;
	//OutColor = vec4(TextureColor, 1.0);
	// Definindo cor via UV
	//OutColor = vec4(UV,0.0 ,1.0);
	//Definindo uma cor
	//OutColor = vec4(Color, 1.0);
}