#include "../headers/camera.hpp"

FlyCamera::FlyCamera(){}
FlyCamera::FlyCamera(int width, int heigth){
    this->aspectRatio = width/heigth;
}

void FlyCamera::moveForward(float qtd){
    this->location += glm::normalize(direction) * qtd * speed;
}

void FlyCamera::moveRight(float qtd){
    //Calcula o produto vetorial
    glm::vec3 right = glm::normalize(glm::cross(direction, up));
    this->location += right * qtd * speed;
}

void FlyCamera::lookAt(float qtdY, float qtdX){
    // Aplicando a velocidade do movimento
    qtdY *= sensitivity;    qtdX *= sensitivity;
    
    
    const glm::mat4 matIdentidade = glm::identity<glm::mat4>();  // Matriz identidade
    glm::vec3 right = glm::normalize(glm::cross(direction, up)); // Produto vetorial
    
    // Criando as matrizes de rotação
    glm::mat4 MatrizRotacaoExioY = glm::rotate(matIdentidade,glm::radians(qtdY),up);
    glm::mat4 MatrizRotacaoExioX = glm::rotate(matIdentidade,glm::radians(qtdX),right);

    // Plicando nos vetores de dire��o
    this->up = MatrizRotacaoExioX * glm::vec4{ this->up , 0.0f };
    this->direction = MatrizRotacaoExioY * MatrizRotacaoExioX * glm::vec4{ this->direction , 0.0f };
}

void FlyCamera::rotate (glm::vec3 axis, float angle){
    glm::mat4 identity   = glm::identity<glm::mat4>();
    glm::mat4 mRotation  = glm::rotate(identity, glm::radians(angle), axis);

    location  = glm::vec3(mRotation * glm::vec4({location, 1.0f}));
    direction = glm::vec3(mRotation * glm::vec4({direction, 1.0f}));
}

glm::mat4 FlyCamera::getProjectionView(){
    glm::mat4 view = glm::lookAt(location, location + direction, up);
    glm::mat4 projection = glm::perspective(fieldOfView, aspectRatio, near, far);
    return projection * view;
}