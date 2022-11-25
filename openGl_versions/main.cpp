#include "./../common.h"

// Definindo as dimenções da janela
const int Width = 800;
const int Height = 600;

int main() {
  setlocale(LC_ALL, "pt_BR");

  assert(glfwInit());                 // Inicializando a biblioteca do GLFW

  // Construindo a janela
  GLFWwindow* window = glfwCreateWindow(Width, Height, "CG com OpenGL", nullptr, nullptr);

  // Verificar se a janela é nula
  assert(window);

  // Definindo a janela de contexto
  glfwMakeContextCurrent(window);

  // inicializa o Glew
  int statusGlwInit = glewInit();
  std::cout << statusGlwInit << endl;
  assert(glewInit() == GLEW_OK);

  // Verigicar a versão do OpenGL que está usando
  GLint GLMajorVersion = 0;
  GLint GLMinorVersion = 0;

  glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
  glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);

  std::cout << "Maior versão do OpenGL suportada: " << GLMajorVersion << endl;
  std::cout << "Menor versão do OpenGL suportada: " << GLMinorVersion << endl;

  // Obtendo informações do driver utilizado
  std::cout << "Fabricante do driver de vídeo:    " << glGetString(GL_VENDOR) << endl;
 //std::cout << "Modelo da placa de vídeo:         " << glGetString(GL_RENDER) << endl;
  std::cout << "Versão do OpenGL:                 " << glGetString(GL_VERSION) << endl;
  std::cout << "Versão do GLS:                    " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

  // Definir cor de fundo
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

  // Construindo o loop de enventos
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);                // limpa o buffer de cor
    glfwPollEvents();                            // Processa todos os eventos na fila do GLFW
    glfwSwapBuffers(window);                     // Envia o conteúdo para desenhar na tela
  }

  glfwTerminate();                               // Finaliza a biblioteca GLFW

  return 0;
}
