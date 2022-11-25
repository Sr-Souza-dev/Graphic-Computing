#include <iostream>
#define GLM_SWIZZLE //Tem que ser antes do Glm
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>



void Construtores() {
	std::cout << "Vector 2" << std::endl;
	glm::vec2 Point0{ 10.0f, 10.0f};
	std::cout << "Point0: " << glm::to_string(Point0) << std::endl;

	std::cout << "Vector 3" << std::endl;
	glm::vec3 Point1{ 10.0f, 0.0f, 0.0f };
	std::cout << "Point1: " << glm::to_string(Point1) << std::endl;

	std::cout << "Vector 4" << std::endl;
	glm::vec4 Point2{ 10.0f, 10.0f, 0.0f, 0.0f };
	std::cout << "Point2: " << glm::to_string(Point2) << std::endl;

	std::cout << "Vector 2 de Inteiros" << std::endl;
	glm::ivec2 Point3{ 3, 4 };
	std::cout << "Point3: " << glm::to_string(Point3) << std::endl;

	std::cout << "Vector 3 de Boleanos" << std::endl;
	glm::bvec3 Point4{ true, false, true};
	std::cout << "Point4: " << glm::to_string(Point4) << std::endl;

	std::cout << "Vector 3 de Doubles" << std::endl;
	glm::dvec3 Point5{ 10.0, 20.0, 10.0};
	std::cout << "Point5: " << glm::to_string(Point5) << std::endl;

	std::cout << "Vector 3" << std::endl;
	glm::vec3 Point6{ Point0, 10.0 };
	std::cout << "Point6: " << glm::to_string(Point6) << std::endl;

}

void Componentes() {

	std::cout << "Vector 3" << std::endl;
	glm::vec3 Ponto{ 1, 2, 3 };
	std::cout << "Ponto: " << glm::to_string(Ponto) << std::endl;

	std::cout << "Ponto Eixo X: " << Ponto.x << std::endl;
	std::cout << "Ponto Eixo Y: " << Ponto.y << std::endl;
	std::cout << "Ponto Eixo Z: " << Ponto.z << std::endl;

	std::cout << "Ponto Eixo X: " << Ponto[0] << std::endl;
	std::cout << "Ponto Eixo Y: " << Ponto[1] << std::endl;
	std::cout << "Ponto Eixo Z: " << Ponto[2] << std::endl;

	std::cout << "Ponto R: " << Ponto.r << std::endl;
	std::cout << "Ponto G: " << Ponto.g << std::endl;
	std::cout << "Ponto Z: " << Ponto.b << std::endl;


}

void  Operacoes() {
	glm::vec3 P0{ 10.0f, 10.0f, 0.0f };
	glm::vec3 P1{ 10.0f, 10.0f, 10.0f };

	std::cout << "P0: " << glm::to_string(P0) << std::endl;
	std::cout << "P1: " << glm::to_string(P1) << std::endl;
	
	//Soma de vetores
	glm::vec3 PSoma = P0 + P1;
	std::cout << "P0 + P1 = " << glm::to_string(PSoma) << std::endl;
	
	//Subtração de vetores
	glm::vec3 PSubtracao = P0 +- P1;
	std::cout << "P0 - P1 = " << glm::to_string(PSubtracao) << std::endl;

	// Transformação de Escala entre dois vetores
	
	// Divisão de vetores
	glm::vec3 PEscala1 = P0 / P1;
	std::cout << "P0 / P1 = " << glm::to_string(PEscala1) << std::endl;

	glm::vec3 PEscala2 = P0 / 10.0f;
	std::cout << "P0 / 10.0f = " << glm::to_string(PEscala2) << std::endl;

	glm::vec3 P2 = P0 / P1;
	std::cout << "P0 / 5.0f = " << glm::to_string(P2) << std::endl;

	// Multiplicação de vetores
	glm::vec3 Pmultiplicacao = P0 * P1;
	std::cout << "P0 * P1 = " << glm::to_string(Pmultiplicacao) << std::endl;

	glm::vec3 Pmultiplicacao2 = P0 * 5.0f;
	std::cout << "P0 * 5.0f = " << glm::to_string(Pmultiplicacao2) << std::endl;

	// Comprimento de um vetor  = raiz quadrada da soma dos quadrados dos eixos
	float ComprimentoP0 = glm::length(P0);
	std::cout << "Comprimeno de P0 = " << ComprimentoP0 << std::endl;
	
	// retorna o números de componentes do vetor
	constexpr int NumeroComponentesP1 = P1.length();
	std::cout << "Total de componenrtes de P1 = " << NumeroComponentesP1 << std::endl;
	
	// Normarmalizar um vetor
	glm::vec3 NormaP0 = glm::normalize(P0);
	std::cout << "Normalização do vetor P0 = " << glm::to_string(NormaP0) << std::endl;

	// Dot product, Produto escalar entre dois vetores
	float ProdudoEscalarP0eP1 = glm::dot(P0, P1);
	std::cout << "Dot product = " << ProdudoEscalarP0eP1 << std::endl;
	
	// Cross product = Produto Vetrial entre dois vetores
	glm::vec3 Cross = glm::cross(P0, P1);
	std::cout << "Cross product = " << glm::to_string(Cross) << std::endl;
	
	// Distancia entre dois pontos
	float DistanciaP0aP1 = glm::distance(P0, P1);
	std::cout << "Distância entre P0 e P1= " << DistanciaP0aP1 << std::endl;

	// Refract = Vetor de refração
	glm::vec3 Refract = glm::refract(P0, NormaP0, 1.0f);
	std::cout << "Refract = " << glm::to_string(Refract) << std::endl;

	// Reflect = Calcula o vetor de reflexão
	glm::vec3 Reflect = glm::reflect(P0, NormaP0);
	std::cout << "Reflect = " << glm::to_string(Reflect) << std::endl;
	
}

void MetodoSwizzle() {

	std::cout << "Vector 3" << std::endl;
	glm::vec3 Ponto{ 1, 2, 3 };
	std::cout << "Ponto: " << glm::to_string(Ponto) << std::endl;

	//Podemos usar atalhos
	//Pra funcionar tem que colocar #define GLM_SWIZZLE
	//antes do #include <glm/glm.hpp> 
	glm::vec3 Ponto2 = Ponto.xxx;
	glm::vec3 Ponto3 = Ponto.xyx;
	//glm::vec3 Ponto4 = Ponto.xyxw; //Pra fazer isso Ponto tem que ser vec4

	std::cout << "Ponto Eixo X: " << Ponto2.x << std::endl;
	std::cout << "Ponto Eixo Y: " << Ponto2.y << std::endl;
	std::cout << "Ponto Eixo Z: " << Ponto2.z << std::endl;




}
int main() {
	setlocale(LC_ALL, "pt_BR");
	
	std::cout << "Exemplo de Vetores" << std::endl;
	
	//Construtores();
	Operacoes();
	return 0;
}
