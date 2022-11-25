#include "../headers/figure.hpp"

// Construtores definition
template <typename VecTP,typename VecTP2, class MatTP>
Figure<VecTP,VecTP2, MatTP>::Figure(){
    identity = glm::identity<MatTP>();
}

template <typename VecTP,typename VecTP2, typename MatTP>
Figure<VecTP,VecTP2, MatTP>::Figure(std::vector<VecTP> figure):figure(figure){
    identity = glm::identity<MatTP>();
}

// Inserção e remoção de pontos
template <typename VecTP,typename VecTP2, typename MatTP>
void Figure<VecTP,VecTP2, MatTP>::addVertex(VecTP vertex){
    figure.push_back(vertex);
}

template <typename VecTP,typename VecTP2, typename MatTP>
void Figure<VecTP,VecTP2, MatTP>::deleteVertex(int index){
    figure.erase(index);
}

// Imprime os vertices da figura
template <typename VecTP,typename VecTP2, typename MatTP>
void Figure<VecTP,VecTP2, MatTP>::printFigure(){
    for(auto vertex : this->figure){
        std::cout<<"| ";
        for(long unsigned int i=0; i<sizeof(vertex)/4; i++){
            std::cout<<std::setw(5)<<vertex[i]<<" | ";
        }   std::cout<<std::endl;
    }  std::cout<<std::endl;
}

// Retorna a figura completa
template <typename VecTP,typename VecTP2, typename MatTP>
std::vector<VecTP> Figure<VecTP,VecTP2, MatTP>::getFigure(){
    return figure;
}

// Converte de vec para vec2
template <typename VecTP,typename VecTP2, typename MatTP>
std::vector<VecTP2> Figure<VecTP,VecTP2, MatTP>::transformVecToVec2(){
    std::vector<VecTP2> fig;
    for(long unsigned int i = 0; i<figure.size(); i++){
        fig.push_back({this->figure[i], 1});
    }

    return fig;
}
// Converte de vec2 para vec
template <typename VecTP,typename VecTP2, typename MatTP>
void  Figure<VecTP,VecTP2, MatTP>::transformVec2ToVec(std::vector<VecTP2> fig){
    for(long unsigned int i = 0; i<fig.size() and i<figure.size(); i++){
        figure[i] = VecTP(fig[i]);
    }
}


template <typename VecTP,typename VecTP2, typename MatTP>
void Figure<VecTP,VecTP2, MatTP>::translation(VecTP factor){

    MatTP mTranslation = glm::translate(this->identity, factor);
    std::vector<VecTP2> figtransla = transformVecToVec2();

    for(auto& vertex : figtransla){
        vertex = mTranslation * (vertex);
    }
    transformVec2ToVec(figtransla);
}

template <typename VecTP,typename VecTP2, typename MatTP>
void Figure<VecTP,VecTP2, MatTP>::scale(VecTP factor){
    MatTP mScale = glm::scale(this->identity, factor);
    std::vector<VecTP2> figScale = transformVecToVec2();

    for(auto& vertex : figScale){
        vertex = mScale * (vertex);
    }
    transformVec2ToVec(figScale);
}

template <typename VecTP,typename VecTP2, typename MatTP>
void Figure<VecTP,VecTP2, MatTP>::rotation(VecTP axis, float angle){

    MatTP mRotation = glm::rotate(this->identity, glm::radians(angle), axis);
    std::vector<VecTP2> figRotation = transformVecToVec2();

    for(auto& vertex : figRotation){
        vertex = mRotation * (vertex);
    }
    transformVec2ToVec(figRotation);
}

template <typename VecTP,typename VecTP2, typename MatTP>
void Figure<VecTP,VecTP2, MatTP>::reflection(VecTP axis){

    for(auto& vertex : this->figure){
        vertex = vertex * axis;
    }
}

template <typename VecTP,typename VecTP2, typename MatTP>
GLFWwindow* Figure<VecTP,VecTP2, MatTP>::initWindow(int width, int height){
    setlocale(LC_ALL, "pt_BR");
    assert(glfwInit());                                                                         // Inicializando a biblioteca do GLFW
    GLFWwindow* window = glfwCreateWindow(width, height, "CG com OpenGL", nullptr, nullptr);    // Construindo a janela
    assert(window);                                                                             // Verificar se a janela é nula
    glfwMakeContextCurrent(window);      
    
    // inicializa o Glew
    assert(glewInit() == GLEW_OK);  
    return window;
}

template <typename VecTP,typename VecTP2, typename MatTP>
void Figure<VecTP,VecTP2, MatTP>::render(GLFWwindow* window){
    glGenBuffers(1, &VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(figure), figure.data(), GL_DYNAMIC_DRAW);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);


    glClear(GL_COLOR_BUFFER_BIT);                   // Limpar o Buffer de cor

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

    glVertexAttribPointer(0, figure.size(), GL_FLOAT, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_POLYGON, 0, figure.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);

    glfwPollEvents();                            // Processa todos os eventos na fila do GLFW
    glfwSwapBuffers(window);                     // Envia o conteúdo para desenhar na tela
}

template<typename VecTP2>
void printAux(std::vector<VecTP2> fig){
    for(auto item : fig){
        for(long unsigned int j = 0; j < sizeof(item)/4; j++)  std::cout<<" | "<<item[j]; 
        std::cout<<std::endl;
    }
}