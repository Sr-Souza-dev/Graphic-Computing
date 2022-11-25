#pragma once
#include <bits/stdc++.h>
#include <GL/glew.h>
#include <cassert>
#include <fstream>

class ShadersConfig{
    public:
    GLuint programID;
    void loadShaders(const char* VertexShaderFile, const char* FragmentShaderFile);

    private:
    std::string loadFile(const char* path);
    void checkShader(GLuint shaderID);
};