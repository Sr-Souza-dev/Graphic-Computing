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
int menu(){
	int opt;
	while(opt != 1 and opt != 2){
		std::cout<<"------ Escolha uma figura para ser desenhada na tela: ------"<<std::endl;
		std::cout<<"Digite 1 para plotar um cubo"<<std::endl;
		std::cout<<"Digite 2 para plotar uma Pirâmide"<<std::endl;
		std::cin>>opt;
	}
	return opt;
}

int main() {
	setlocale(LC_ALL, "pt_BR");				//Inicializando a biblioteca do GLFW 
	assert(glfwInit() == GLFW_TRUE);

	//Construindo a janela
	GLFWwindow* window = glfwCreateWindow(Width, Heigth, "CG com OpenGL", nullptr, nullptr);
	assert(window);							//Verificar se a janela é nula
	glfwMakeContextCurrent(window);			//Definindo a janela de contexto

	//Cadastrar callbacks no GLFW
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, MouseMotionCallback);
	glfwSetFramebufferSizeCallback(window, resize);
	//resize(window, Width, Heigth);

	//inicializa o GLEW
	int statusGlwInit = glewInit();
	assert(glewInit() == GLEW_OK);

	//Carregando os Shaders
	shaders.loadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");

	// Seleciona qual figura plotar
	switch(menu()){
		case 1:
			figure.setCube(1.0);							//Carregando a geometria
		break;
		case 2:
			figure.setPyramid(1.0);							//Carregando a geometria
		break;
	}

	
	glm::mat4 MatrizModel = glm::identity<glm::mat4>();		//Criando o Model
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);					//Definir cor de fundo	
	double  lastFramUpdate = glfwGetTime();					//Tempo de atualizado dos frames
	
	glEnable(GL_DEPTH_TEST);		// Enable depth test
	glDepthFunc(GL_LESS);			// Accept fragment if it closer to the camera than the former one
	
	//Construindo o loop de eventos
	while (!glfwWindowShouldClose(window)) {
		//Calculando o DeltaTime
		double  currentFrameUpdate = glfwGetTime();
		double DeltaTime = currentFrameUpdate - lastFramUpdate;
		if (DeltaTime > 0) {lastFramUpdate = currentFrameUpdate;}
		
		glClear(GL_COLOR_BUFFER_BIT);		// Limpar o Buffer de cor
		glUseProgram(shaders.programID);	// Ativar o Shader criado
		
		// Calculando a MVP dinamicamente
		glm::mat4 MatrizViewProjection = Camera.getProjectionView();
		glm::mat4 ModelViewProjection = MatrizViewProjection * MatrizModel;

		GLint ModelViewProjectionLoc = glGetUniformLocation(shaders.programID, "ModelViewProjection");
		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

		
		glBindVertexArray(figure.VAO);				   //Passando o objeto que será desenhado
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	   //Definindo o modo de desenho

		//Desenhar os pontos
		glDrawElements(GL_TRIANGLES, figure.getFig().vertices.size()*6,GL_UNSIGNED_INT, nullptr);
		moveKey(window, DeltaTime);

		//Desabilita o programa
		glBindVertexArray(0);		//Desabilitando o glBindVertexArray(figure.VAO), pq já desenhou
		glUseProgram(0);
		glfwPollEvents();
		glfwSwapBuffers(window);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	}

	glDeleteVertexArrays(1, &figure.VAO);
	glfwTerminate();
	return 0;
}