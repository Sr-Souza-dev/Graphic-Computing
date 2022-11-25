#include "./../common.h"

const int Width = 800;
const int Height = 600;

int main(){
    setlocale(LC_ALL, "pt_BR");
    assert(glfwInit());                                                                         // Inicializando a biblioteca do GLFW
    GLFWwindow* window = glfwCreateWindow(Width, Height, "CG com OpenGL", nullptr, nullptr);    // Construindo a janela
    assert(window);                                                                             // Verificar se a janela é nula
    glfwMakeContextCurrent(window);      
    
    // inicializa o Glew
    int statusGlwInit = glewInit();
    assert(glewInit() == GLEW_OK);                                                              // Definindo a janela de contexto

    // Definindo uma figura
    vector<glm::vec3> figure3D = {
        glm::vec3{-1.0f, -1.0f, 0.0f},
        glm::vec3{ 1.0f, -1.0f, 0.0f},
        glm::vec3{ 0.0f,  1.0f, 0.0f},
    };

    GLuint VertexBuffer;
    glGenBuffers(1, &VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(figure3D), figure3D.data(), GL_STATIC_DRAW);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);                   // Limpar o Buffer de cor

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

        glVertexAttribPointer(0, figure3D.size(), GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, figure3D.size());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(0);

        glfwPollEvents();                            // Processa todos os eventos na fila do GLFW
        glfwSwapBuffers(window);                     // Envia o conteúdo para desenhar na tela
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}