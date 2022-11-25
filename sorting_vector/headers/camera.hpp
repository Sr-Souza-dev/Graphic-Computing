#pragma once

#include <bits/stdc++.h>
//Biblioteca do OpenGL
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>


class FlyCamera {
private:
    //Parametros de Interatividade, define a velocidade do movimento
	float speed = 1;
	float sensitivity = 0.1f;

	//Parametros da Matriz de view
	glm::vec3 location  { 0.0f, 0.0f,  6.0f };
	glm::vec3 direction { 0.0f, 0.0f, -1.0f };
	glm::vec3 up 		{ 0.0f, 1.0f,  0.0f };

	//Par�metros da Matriz Projection
	float fieldOfView = glm::radians(45.0f);
	float near = 0.01f;
	float far = 1000.0f;

public:
	float aspectRatio;
	
    FlyCamera();
    FlyCamera(int width, int heigth);

    void moveForward	(float qtd);					// Movimenta a camera para frente ou para trás
    void moveRight		(float qtd);					// Movimenta a camera para direita ou para esquerda
	void rotate			(glm::vec3 axis, float angle);	// Realiza rotação em torno do centro
    void lookAt			(float qtdY, float qtdX);
    glm::mat4 getProjectionView();						// Retorna a matriz de visualização modificada
};

