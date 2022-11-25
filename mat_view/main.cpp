#include "./../common.h"

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
	cout << statusGlwInit << endl;
	assert(glewInit() == GLEW_OK);
	
	//Verificar a versão do OpenGL que está sendo usando
	GLint GLMajorVersion = 0;
	GLint GLMinorVersion = 0;

	glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);

	cout << "Maior versão do OpenGL suportada:" << GLMajorVersion << endl;
	cout << "Menor versão do OpenGL suportada:" << GLMinorVersion << endl;

	//Obtendo informações do driver utilizado
	cout << "Fabircante do driver de video:" << glGetString(GL_VENDOR) << endl;
	cout << "Modelo da placa de video:" << glGetString(GL_RENDERER) << endl;
	cout << "Versão do OpenGL:" << glGetString(GL_VERSION) << endl;
	cout << "Versão do GLS:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	//PSSO 1: Definir um triângulo em coordenadas normalizadas
	array<glm::vec3, 3> Triangulo = {
		glm::vec3{-1.0f, -1.0f, 0.0f},
		glm::vec3{ 1.0f, -1.0f, 0.0f},
		glm::vec3{ 0.0f,  1.0f, 0.0f},
	};

	//Aplicando o MVP
	// Criando o Model
	glm::mat4 MatrizModel = glm::identity<glm::mat4>();

	// Criando a View
	glm::vec3 Eye{ 10, 10, 10 };
	glm::vec3 Center{ 0, 0, 0 };
	glm::vec3 Up{ 0, 1, 0 };
	glm::mat4 MatrizView = glm::lookAt(Eye, Center, Up);

	//Criando a Projection
	constexpr float FoV = glm::radians(45.0f);
	const float AspectioRate = Width / Heigth;
	const float Near = 0.01f;
	const float Far = 1000.0f; 
	glm::mat4 MatrizProjection = glm::perspective(FoV, AspectioRate, Near, Far);

	// Gerando o ModelViewProjection
	// Usando a CPU para calculo, mas o ideal é usar a GPU
	glm::mat4 ModelViewProjection = MatrizProjection * MatrizView * MatrizModel;

	//Aplicando o MVP no triangulo
	for (glm::vec3& Vertice : Triangulo) {
		glm::vec4 VerticeProjetado = ModelViewProjection * glm::vec4{ Vertice , 1 };

		// calculando a coordenada homogena 
		VerticeProjetado /= VerticeProjetado.w;
		Vertice = VerticeProjetado;
	}
	//FIM - PSSO 1
	
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangulo), Triangulo.data(), GL_STATIC_DRAW);
	//PASSO 2 - FIM

	//Definir cor de fundo
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	//Construindo o loop de eventos
	while (!glfwWindowShouldClose(window)) {
		// Limpar o Buffer de cor
		glClear(GL_COLOR_BUFFER_BIT);

		glEnableVertexAttribArray(0);

		// glBindBuffer(TIPO, BUFFER CRIADO);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		
		// glVertexAttribPointer(INDICE, QUANTIDADE DE VERTICES, TIPO
		//							, SE AS POCIÇÕES ESTÃO NORMALIZADAS, USADOS EM TEXTURAS
		//							, PONTEIRO);
		glVertexAttribPointer(0, Triangulo.size(), GL_FLOAT, GL_FALSE, 0, nullptr);
		
		// glDrawArrays(QUE OBJETO SERÁ DESENHADO, INDICE DO OBJETO
		//				, QUANTIDADE DE VERTICES PRA DESENHAR);
		glDrawArrays(GL_TRIANGLES, 0, Triangulo.size());
		
		//Desliga o glBindBuffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//Desalocar o VertexBuffer da memória de video
	glDeleteBuffers(1, &VertexBuffer);

	//Finaliza a biblioteca GLFW
	glfwTerminate();
	return 0;
}