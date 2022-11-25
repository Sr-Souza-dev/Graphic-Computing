#version 330 core

uniform sampler2D TextureSampler;

in vec3 Color;
in vec2 UV;
// Declarando uma variável de saída do tipo vec4
out vec4 OutColor;

void main(){
	vec3 TextureColor = texture(TextureSampler, UV).rgb;

	// Mesclando a Cor RGB informada com a textura,
	// Note que é uma multiplicação de matrizes
	vec3 FinalColor = Color * TextureColor;
	OutColor = vec4(TextureColor, 1.0);

	//Para inverter a cor
	//vec3 FinalColor = -1.0 * Color * TextureColor;
	//OutColor = vec4(TextureColor, 1.0);
	// Definindo cor via UV
	//OutColor = vec4(UV,0.0 ,1.0);
	//Definindo uma cor
	//OutColor = vec4(Color, 1.0);
}