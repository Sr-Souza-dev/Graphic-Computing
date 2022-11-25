#include "headers/shaders.hpp"
#include "headers/figure.hpp"
#include "headers/motionFuncs.hpp"

// W -> aproxima
// S  -> afasta
// A  -> move para esquerda
// D  -> move para direita
// Q  -> rotaciona para esquerta
// E  -> rotaciona para direita
// R  -> rotaciona para cima (vertical)
// T  -> rotaciona para baixo (vertical)

ShadersConfig shaders;
Figure figure;

float fabs(float value){
	return value<0 ? value*-1 : value;
}

int main() {
	setlocale(LC_ALL, "pt_BR");								// Inicializando a biblioteca do GLFW 
	assert(glfwInit() == GLFW_TRUE);

	std::vector<int> vec;
	int qtdVec; std::cout<<"Digite a quantidade de elementos dentro do vetor: "; std::cin>>qtdVec;
	for(int i=0; i<qtdVec; i++){vec.push_back(rand());}

	// Construindo a janela
	GLFWwindow* window = glfwCreateWindow(Width, Heigth, "CG com OpenGL", nullptr, nullptr);
	assert(window);											// Verificar se a janela é nula
	glfwMakeContextCurrent(window);							// Definindo a janela de contexto

	// Cadastrar callbacks no GLFW
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, MouseMotionCallback);
	glfwSetFramebufferSizeCallback(window, resize);

	// inicializa o GLEW
	int statusGlwInit = glewInit();
	assert(glewInit() == GLEW_OK);

	figure.setVectorSorting(vec);							// Carregando a geometria

	// Carregando os Shaders
	shaders.loadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");
	
	glm::mat4 MatrizModel = glm::identity<glm::mat4>();		// Criando o Model
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);					// Definir cor de fundo	
	double  lastFramUpdate = glfwGetTime();					// Tempo de atualizado dos frames
	
	glEnable(GL_DEPTH_TEST);								// Enable depth test
	glDepthFunc(GL_LESS);									// Accept fragment if it closer to the camera than the former one

	int interaction = 0; float eps = 0.1/vec.size(), step = 0.07, cstep;
	std::vector<std::pair<float, float>> swapInf = {{0,0}, 
													{figure.getFig().vertices[0].position[0], 0}, 
													{figure.getFig().vertices[0].position[0], 0}}; 
	
	// Construindo o loop de eventos
	while (!glfwWindowShouldClose(window)) {
		//Calculando o DeltaTime
		double  currentFrameUpdate = glfwGetTime();
		double DeltaTime = currentFrameUpdate - lastFramUpdate;
		if (DeltaTime > 0) {lastFramUpdate = currentFrameUpdate;}
		
		glClear(GL_COLOR_BUFFER_BIT);						// Limpar o Buffer de cor
		glUseProgram(shaders.programID);					// Ativar o Shader criado
		
		// Calculando a MVP dinamicamente
		glm::mat4 MatrizViewProjection = Camera.getProjectionView();
		glm::mat4 ModelViewProjection = MatrizViewProjection * MatrizModel;

		GLint ModelViewProjectionLoc = glGetUniformLocation(shaders.programID, "ModelViewProjection");
		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

		
		glBindVertexArray(figure.VAO);				   		// Passando o objeto que será desenhado
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	   		// Definindo o modo de desenho

		// Desenhar os pontos
		glDrawElements(GL_TRIANGLES, figure.getFig().vertices.size()*6,GL_UNSIGNED_INT, nullptr);
		moveKey(window, DeltaTime);

		// Desabilita o programa
		glBindVertexArray(0);						   		// Desabilitando o glBindVertexArray(figure.VAO), pq já desenhou
		glUseProgram(0);
		glfwPollEvents();
		glfwSwapBuffers(window);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		// Realiza a ordenação do Vetor
		if( fabs(figure.getFig().vertices[swapInf[0].first ].position[0] - swapInf[2].first) < eps and
			fabs(figure.getFig().vertices[swapInf[0].second].position[0] - swapInf[1].first) < eps and
			interaction < vec.size()){

			if((int)swapInf[0].first != (int)swapInf[0].second){
				figure.swapVertex(swapInf[0].first, swapInf[0].second);
			}
			swapInf = figure.sortByStep(interaction);
			interaction++;
		}
		else if (interaction < vec.size() and (int)swapInf[0].first != (int)swapInf[0].second){
			cstep = fabs(figure.getFig().vertices[swapInf[0].first ].position[0] - swapInf[2].first) < 1.2*step ? eps/2 : step;
			figure.swapFig(swapInf[0].first, swapInf[0].second, cstep);
			figure.loadFigure();
		}
	}

	glDeleteVertexArrays(1, &figure.VAO);
	glfwTerminate();
	return 0;
}