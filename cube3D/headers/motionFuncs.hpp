#include "camera.hpp"
#include <GLFW/glfw3.h>             // Biblioteca que ira desenhar na tela

//Definindo as dimenções da janela
const int Width = 800;
const int Heigth = 600;

FlyCamera Camera(Width, Heigth);

glm::vec2 cursorLastPosition;
bool mouseMovieActive = false;

// Função de callback, tem que ter estrutura padrão
void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Modifiers){
	//Essa função retorna quais botoes do mouse foram clicados.
	//Verifica se é botão esquerdo do mouse
	if (Button == GLFW_MOUSE_BUTTON_LEFT){	
		//Verifica qual a a��o relacizada com o bot�o esquerdo
		if (Action == GLFW_PRESS){
			//Desabilita o curso quando estiver fora da window
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			double X, Y;
			//Retorna a posi��o do mouse na tela
			glfwGetCursorPos(Window, &X, &Y);
			cursorLastPosition = glm::vec2{ X, Y };
			mouseMovieActive = true;
		}
		// Verifica se a a��o foi encerrada
		else if (Action == GLFW_RELEASE){
			//Habilita novamente o cursos, para ter o funcionamento normal
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			mouseMovieActive = false;
		}
	}
}

//Fun��o de callback, tem que ter estrutura padr�o
void MouseMotionCallback(GLFWwindow* Window, double X, double Y) {
	//Essa função retorna a posição do mouse na tela
	if (mouseMovieActive){
		glm::vec2 PosicaoAtualCursor{ X, Y };
		glm::vec2 Delta = cursorLastPosition - PosicaoAtualCursor;

		Camera.lookAt(-Delta.x, -Delta.y);
		cursorLastPosition = PosicaoAtualCursor;
	}
}

void resize(GLFWwindow* window, int newWidth, int newHeigth){
	Camera.aspectRatio = static_cast<float> (newWidth) / newHeigth;
}

void moveKey(GLFWwindow* window, double DeltaTime){
    // Capturando eventor do teclado
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {Camera.moveForward ( 1.0f * DeltaTime);}
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {Camera.moveForward (-1.0f * DeltaTime);}
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {Camera.moveRight (-1.0f * DeltaTime);}
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {Camera.moveRight ( 1.0f * DeltaTime);}
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {Camera.rotate ({ 0,  1, 0}, 0.5);}
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {Camera.rotate ({ 0, -1, 0}, 0.5);}
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {Camera.rotate ({ 1,  0, 0}, 0.5);}
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {Camera.rotate ({-1,  0, 0}, 0.5);}
}
