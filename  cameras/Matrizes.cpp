#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

void imprimirFormatado(const glm::mat4& M) {
	std::cout << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout
				<< std::setw(10)
				<< std::setprecision(4)
				<< std::fixed
				<< M[i][j] << "  ";
		}
		std::cout << std::endl;
	}
}

void Translacao() {

	//Construindo uma Matriz identidade
	glm::mat4 MIdentidade = glm::identity<glm::mat4>();
	std::cout << "Matriz Identidade" << std::endl;
	imprimirFormatado(MIdentidade);

	//Construindo uma Matriz de Translação
	glm::vec3 FatorTranslacao{ 10.0f, 10.0f, 10.0f };
	std::cout << "Fator de translação: " << glm::to_string(FatorTranslacao) << std::endl;

	glm::mat4 MTranslacao = glm::translate(MIdentidade, FatorTranslacao);
	std::cout << "Matriz de Translação" << std::endl;
	imprimirFormatado(MTranslacao);

	//Aplicando a translação no ponto
	glm::vec4 Posicao1{ 10,10,10,1 };
	std::cout << "Posicao1 original: " << glm::to_string(Posicao1) << std::endl;
	glm::vec4 Direcao1{ 10,10,10,0 };

	glm::vec4 Posicao1Linha = MTranslacao * Posicao1;
	std::cout << "Posicao1 transladado:: " << glm::to_string(Posicao1Linha) << std::endl;

}

void Escala() {
	//Construindo uma Matriz identidade
	glm::mat4 MIdentidade = glm::identity<glm::mat4>();
	std::cout << "Matriz Identidade" << std::endl;
	imprimirFormatado(MIdentidade);

	//Construindo uma Matriz de Escala
	glm::vec3 FatorEscala{ 2, 2, 2};
	std::cout << "Fator de escala: " << glm::to_string(FatorEscala) << std::endl;

	glm::mat4 MEscala = glm::scale(MIdentidade, FatorEscala);
	std::cout << "Matriz de Escala" << std::endl;
	imprimirFormatado(MEscala);

	//Aplicando a escala no ponto
	glm::vec4 Posicao1{ 100,100,0,1 };
	std::cout << "Posicao1 original: " << glm::to_string(Posicao1) << std::endl;
	glm::vec4 Direcao1{ 100,100,0,0 };

	Posicao1 = MEscala * Posicao1;
	std::cout << "Posicao1 Escalado: " << glm::to_string(Posicao1) << std::endl;
}

void Rotacao() {
	//Construindo uma Matriz identidade
	glm::mat4 MIdentidade = glm::identity<glm::mat4>();
	std::cout << "Matriz Identidade" << std::endl;
	imprimirFormatado(MIdentidade);

	// Construindo uma Matriz de Rotação
	constexpr float AnguloDeRotacao = glm::radians(90.0f);//90 Graus
	glm::vec3 EixoDeRotacao{ 0, 0, 1 };//Rotacionar no eixo Z
	glm::mat4 MRotacao = glm::rotate(MIdentidade, AnguloDeRotacao, EixoDeRotacao);
	std::cout << "Matriz de Rotação" << std::endl;
	imprimirFormatado(MRotacao);

	//Aplicando a rotação no ponto
	glm::vec4 Posicao1{ 100,0,0,1 };
	std::cout << "Posicao1 original: " << glm::to_string(Posicao1) << std::endl;
	glm::vec4 Direcao1{ 100,0,0,0 };

	Posicao1 = MRotacao * Posicao1;
	std::cout << "Posicao1 Rotacionado: " << glm::to_string(Posicao1) << std::endl;
}

void Composicao() {
	
	glm::mat4 MIdentidade = glm::identity<glm::mat4>();
	std::cout << "Matriz Identidade" << std::endl;
	imprimirFormatado(MIdentidade);

	//Construindo uma Matriz de Translação
	glm::vec3 FatorTranslacao{ 0, 10 , 0 };
	glm::mat4 MTranslacao = glm::translate(MIdentidade, FatorTranslacao);

	// Construindo uma Matriz de Rotação
	constexpr float AnguloDeRotacao = glm::radians(45.0f);
	glm::vec3 EixoDeRotacao{ 0, 0, 1 };
	glm::mat4 MRotacao = glm::rotate(MIdentidade, AnguloDeRotacao, EixoDeRotacao);

	//Construindo uma Matriz de Escala
	glm::vec3 FatorEscala{ 2, 2, 0 };
	glm::mat4 MEscala = glm::scale(MIdentidade, FatorEscala);


	std::cout << "Matriz de Translação" << std::endl;
	imprimirFormatado(MTranslacao);

	std::cout << "Matriz de Rotação" << std::endl;
	imprimirFormatado(MRotacao);

	std::cout << "Matriz de Escala" << std::endl;
	imprimirFormatado(MEscala);

	// Agurpamentos das Tranformações
	// Atenção na ordem, é multiplicação de matrizes
	glm::mat4 MTransformacao = MTranslacao * MRotacao * MEscala;
	std::cout << "Matriz de Transformação" << std::endl;
	imprimirFormatado(MTransformacao);

	glm::vec4 Posicao{ 1,1,0,1 };
	glm::vec4 Direcao{ 1,1,0,0 };

	Posicao = Posicao * MTransformacao;
	Direcao = Direcao * MTransformacao;

	std::cout << "Posicao: " << glm::to_string(Posicao) << std::endl;
	std::cout << "Direcao: " << glm::to_string(Direcao) << std::endl;
}

// Matriz de Projeção e visualização
void ModelViewProject() {

	//É a matriz formada pelas as transformações Transfalação, Rotação e Escala
	glm::mat4 MatrizModel = glm::identity<glm::mat4>();
	
	// Matriz de Visualização
	glm::vec3 Eye{ 0, 0, 10}; // Posição do Olho
	glm::vec3 Center{0, 0, 0};// Para onde esta olhando
	glm::vec3 Up{0, 1, 0};// Inclinação

	glm::mat4 MatrizView = glm::lookAt(Eye, Center, Up);
	std::cout << "Matriz de Visualização" << std::endl;
	imprimirFormatado(MatrizView);

	// Matriz de projeção
 	constexpr float FoV = glm::radians(45.0f);// Angulo de abertura da visão
	const float AspectioRate = 800.0f / 600.0f;  // Proporção da Tela Largura/Altura
	const float Near = 1.0f;  // A menor distancia pra começar a ver as coisas
	const float Far = 100000.0f;  // A maior distancia pra contiar a ver as coisas

	glm::mat4 MatrizProjecao = glm::perspective(FoV, AspectioRate, Near, Far);

	std::cout << "Matriz de Projeção" << std::endl;
	imprimirFormatado(MatrizProjecao);

	// Gerando o modelo completo
	glm::mat4 ModelViewProjection = MatrizProjecao * MatrizView * MatrizModel;

	std::cout << "Matriz MVP" << std::endl;
	imprimirFormatado(ModelViewProjection);

	// Aplicando o MVP em um ponto qualquer
	glm::vec4 Ponto{ 0,0,0,1 };
	std::cout << "Ponto Original: " << glm::to_string(Ponto) << std::endl;

	Ponto = ModelViewProjection * Ponto;
	std::cout << "Ponto Projetado (Coordenada Homogenia): " << glm::to_string(Ponto) << std::endl;

}
int main() {
	setlocale(LC_ALL, "pt_BR");
	std::cout << "Exemplo de Matrizes" << std::endl;
	ModelViewProject();
	return 0;
}
