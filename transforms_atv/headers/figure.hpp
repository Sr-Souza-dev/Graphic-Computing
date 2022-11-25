#pragma once

/*=======STANDARD TEMPLATE LIBRARY=======*/
#include <bits/stdc++.h>
typedef long long ll;

/*=======GLU/GLEW=======*/
#include <GL/glew.h>
#include <stb/stb_image.h>
#include <glm/ext.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>   // Biblioteca que irá desenhar a tela
#include <cassert>        // Biblioteca de validação


template<typename VecTP, typename VecTP2, typename MatTP> 

class Figure{
    private:
    std::vector<VecTP> figure;
    MatTP         identity;
    GLuint        VertexBuffer;

    std::vector<VecTP2> transformVecToVec2();
    void                transformVec2ToVec(std::vector<VecTP2> fig);

    public:
    Figure ();
    Figure (std::vector<VecTP> figure);

    void addVertex      (VecTP vertex);
    void deleteVertex   (int index);
    void printFigure    ();

    std::vector<VecTP> getFigure();

    void translation    (VecTP factor);
    void scale          (VecTP factor);
    void rotation       (VecTP axis, float angle);
    void reflection     (VecTP axis);

    GLFWwindow* initWindow  (int width, int height);
    void        render      (GLFWwindow* window);
};
