#include "src/figure.cpp"

const int Width = 800;
const int Height = 600;

int main(void) {

  std::vector<glm::vec3> figure3D = {
    glm::vec3{-1.0f, -1.0f, 0.0f},
    glm::vec3{ 1.0f, -1.0f, 0.0f},
    glm::vec3{ 0.0f,  1.0f, 0.0f},
  };

  Figure<glm::vec3, glm::vec4, glm::mat4> fig(figure3D);
  GLFWwindow* window = fig.initWindow(Width, Height);    // Construindo a janela

  long unsigned int cont = 0;
  int step = 1;

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

  while(!glfwWindowShouldClose(window)){
    fig.render(window);

    if(cont == 1e2) {
      step = -1;
      fig.scale({-3.0f, -3.0f, -3.0f});
      fig.reflection({1.0f, -1.0f, 1.0f});
    }
    else if (cont == 5) {
      step = 1;
      fig.scale({0.2f, 0.2f, 0.2f});
      fig.reflection({-1.0f, 1.0f, 1.0f});
    }
    cont += step;
    
    if(step == -1){
      fig.translation({0.0f, 0.0f, 0.1f});
      fig.rotation({0.0f, 0.0f, 1.0f}, -2.0f);
    }else{
      fig.translation({0.0f, 0.0f, -0.1f});
      fig.rotation({0.0f, 0.0f, 1.0f}, 2.0f); 
    }
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}