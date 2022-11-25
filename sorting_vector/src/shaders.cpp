#include "../headers/shaders.hpp"

std::string ShadersConfig::loadFile(const char* path){
    std::string data;
    if (std::ifstream FileStream{path , std::ios::in}) {
        data.assign(std::istreambuf_iterator<char>(FileStream), std::istreambuf_iterator<char>());
    }
    return data;
}

void ShadersConfig::checkShader(GLuint shaderID){
    GLint res = GL_TRUE;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &res);

    if (res == GL_FALSE) {
        GLint logSize = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize);
        
        if (logSize > 0) {
            std::string textoLog(logSize, '\0');
            glGetShaderInfoLog(shaderID, logSize, nullptr, &textoLog[0]);
            std::cout << "Erro no shader:" << std::endl;
            std::cout << textoLog << std::endl;
            assert(false);
        }
    }
}

void ShadersConfig::loadShaders(const char* VertexShaderFile, const char* FragmentShaderFile){
    //Carregando o c�digo fonte do shaders
    std::string VertexShaderSource   = loadFile(VertexShaderFile);
    std::string FragmentShaderSource = loadFile(FragmentShaderFile);

    //Valida se carregou os arquivos corretamente
    assert(!VertexShaderSource.empty());
    assert(!FragmentShaderSource.empty());

    //Criando os identificadores do shaders
    GLuint VertexShaderId   = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    //Compilando os shaders
    const char* VertexShaderSourcePtr = VertexShaderSource.c_str();
    glShaderSource(VertexShaderId, 1, &VertexShaderSourcePtr, nullptr);
    glCompileShader(VertexShaderId);

    // verifica se a compila��o deu certo
    checkShader(VertexShaderId);

    //Compilando os shaders
    const char* FragmentShaderPtr = FragmentShaderSource.c_str();
    glShaderSource(FragmentShaderId, 1, &FragmentShaderPtr, nullptr);
    glCompileShader(FragmentShaderId);

    // verifica se a compila��o deu certo
    checkShader(FragmentShaderId);

    // Gerar o programa
    GLuint programID = glCreateProgram();
    glAttachShader(programID, VertexShaderId);
    glAttachShader(programID, FragmentShaderId);
    glLinkProgram(programID);

    //Verificar se o programa foi constru�do corretamente
    GLint Result = GL_TRUE;
    glGetProgramiv(programID, GL_LINK_STATUS, &Result);

    if (Result == GL_FALSE) {
        // Obter o motivo de n�o funcionar
        GLint tamanhoLog = 0;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &tamanhoLog);

        if (tamanhoLog > 0) {
            std::string textoLog(tamanhoLog, '\0');
            glGetProgramInfoLog(programID, tamanhoLog, nullptr, &textoLog[0]);

            std::cout << "Erro no Program:" << std::endl;
            std::cout << textoLog << std::endl;

            assert(false);
        }
    }

    //Excluindo tudo
    glDetachShader(programID, VertexShaderId);
    glDetachShader(programID, FragmentShaderId);

    glDeleteShader(VertexShaderId);
    glDeleteShader(FragmentShaderId);

    this->programID = programID;
}