#include <iostream>
//Biblioteca do OpenGL
#include <GL/glew.h>
//Biblioteca que irá desenhar a tela
#include <GLFW/glfw3.h>
//Biliotecas de validação
#include <cassert>
#include <array>
#include <glm/glm.hpp>

//Definindo as dimenções da janela
const int Width = 800;
const int Heigth = 600;

int main() {
	setlocale(LC_ALL, "pt_BR");
	//Inicializando a biblioteca do GLFW 
	assert(glfwInit() == GLFW_TRUE);

	//Construindo a janela
	GLFWwindow* window = glfwCreateWindow(Width, Heigth, "CG com OpenGL", nullptr, nullptr);
	//Verificar se a janela é nula
	assert(window);

	//Definindo a janela de contexto
	glfwMakeContextCurrent(window);

	//inicializa o GLEW
	int statusGlwInit = glewInit();
	std::cout << statusGlwInit << std::endl;
	assert(glewInit() == GLEW_OK);
	
	//Verificar a versão do OpenGL que está sendo usando
	GLint GLMajorVersion = 0;
	GLint GLMinorVersion = 0;

	glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);

	std::cout << "Maior versão do OpenGL suportada:" << GLMajorVersion << std::endl;
	std::cout << "Menor versão do OpenGL suportada:" << GLMinorVersion << std::endl;

	//Obtendo informações do driver utilizado
	std::cout << "Fabircante do driver de video:" << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Modelo da placa de video:" << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Versão do OpenGL:" << glGetString(GL_VERSION) << std::endl;
	std::cout << "Versão do GLS:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	//PSSO 1: Definir um triângulo em coordenadas normalizadas
	std::array<glm::vec3, 3> Triangle = {
		glm::vec3{-1.0f, -1.0f, 0.0f},
		glm::vec3{ 1.0f, -1.0f, 0.0f},
		glm::vec3{ 0.0f,  1.0f, 0.0f},
	};
	
	//PASSO 2: copiar os vértices do triângulo para a memória da GPU

	// Variável que vai conter o identificador do buffer de vértices
	GLuint VertexBuffer;

	// Gerar um identificador glGenBuffers(NUMERO DE BUFFER, INDETIFICADOR);
	glGenBuffers(1, &VertexBuffer);

	//Ativar o buffer de vértices, comandos vão afetar o VertexBuffer.
	//O buffer para onde vamos copiar os vertices do triangulo.
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	// Passa para o OpenGL o ponteiro para os dados que serão copiados para GPU
	// basicamente copia os dados para a memória de vídeo
	// glBufferData(ORIGEM DOS DADOS, TAMANHO EM BYTES, PONTEIRO PARA OS DADOS
	//				, TIPO DE USO DO BUFFER);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle.data(), GL_STATIC_DRAW);


	//Definir cor de fundo
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	//Construindo o loop de eventos
	while (!glfwWindowShouldClose(window)) {
		// Limpar o Buffer de cor
		glClear(GL_COLOR_BUFFER_BIT);

		// DESENHAR INICIO
		// Habilita o atributo na posição 0, normalmente é o atributo de vértices
		// Esse vai ser o identificador que vamos usar no shader para ler a posição
		// de cada vértice.
		glEnableVertexAttribArray(0);

		// Diz para o OpenGL que o VertexBuffer vai ficar associado ao atributo 0
		// glBindBuffer(TIPO, BUFFER CRIADO);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

		// Informa ao OpenGL onde, dentro do VertexBuffer, os vértices estão.
		// Neste exemplo  o array Triangles é tudo
		// glVertexAttribPointer(INDICE, QUANTIDADE DE VERTICES, TIPO
		//							, SE AS POCIÇÕES ESTÃO NORMALIZADAS, USADOS EM TEXTURAS
		//							, PONTEIRO);
		glVertexAttribPointer(0, Triangle.size(), GL_FLOAT, GL_FALSE, 0, nullptr);

		// Por fim, diz para o OpenGL desenhar o triângulo
		// glDrawArrays(QUE OBJETO SERÁ DESENHADO, INDICE DO OBJETO
		//				, QUANTIDADE DE VERTICES PRA DESENHAR);
		glDrawArrays(GL_TRIANGLES, 0, Triangle.size());

		//Como boa pratica devemos Desliga o glBindBuffer e desabilitar o atributo da posição
		//Desliga o glBindBuffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//desabilitar o atributo da posição
		glDisableVertexAttribArray(0);
		// DESENHAR FIM
		
		//Processa todos os eventos na fila do GLFW
		glfwPollEvents();
		//Envia o conteúdo para desenhar na tela
		glfwSwapBuffers(window);
	}

	//Desalocar o VertexBuffer da memória de video
	glDeleteBuffers(1, &VertexBuffer);

	//Finaliza a biblioteca GLFW
	glfwTerminate();
	return 0;
}