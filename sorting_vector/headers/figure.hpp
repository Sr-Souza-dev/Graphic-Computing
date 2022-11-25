#pragma once

#include <bits/stdc++.h>            //Default c++

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>                // Biblioteca openGL
#include <cassert>                  // Biblioteca de validação
#include <GLFW/glfw3.h>             // Biblioteca que ira desenhar na tela

struct Vertex{
    glm::vec3 position;
    glm::vec3 color;
};

struct Fig{
    std::vector<Vertex> vertices;
    std::vector< glm::ivec3>order;
};

const std::vector<glm::vec3>  colors = {
    glm::vec3{1.0f, 0.5f, 0.0f},  // 0. laranja
    glm::vec3{1.0f, 0.0f, 1.0f},  // 1. violeta
    glm::vec3{0.0f, 1.0f, 0.0f},  // 2. verde
    glm::vec3{0.0f, 0.0f, 1.0f},  // 3. azul
    glm::vec3{1.0f, 0.0f, 0.0f},  // 4. vermelho
    glm::vec3{1.0f, 1.0f, 0.0f},  // 5. amarelo
    glm::vec3{1.0f, 0.0f, 1.0f},  // 1. violeta
    glm::vec3{0.0f, 0.0f, 1.0f},  // 3. azul
};

class Figure{
    private:
    Fig form;
    std::vector<int> vec;
    void normalizeVertices (int maxWidth, int maxHeight);

    public:
    GLuint VAO;

    void loadFigure();
    void setCube    (float d);
    void setPyramid (float d);
    void printd     ();
    Fig  getFig     ();

    void translation      (glm::vec3 factor);
    void scale            (glm::vec3 factor);
    void rotation         (glm::vec3 axis, float angle);
    void reflection       (glm::vec3 axis);  

    void setVectorSorting (std::vector<int> vec);
    void swapFig          (int first, int second, float step);
    void printVec         ();
    void swapVertex       (int first, int second);

    std::vector<std::pair<float,float>> sortByStep(int interaction);
};