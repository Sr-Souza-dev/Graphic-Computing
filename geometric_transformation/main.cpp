#include "./../common.h"

const int Width = 800;
const int Height = 600;

void printFormat(const glm::mat4 & M){
    cout<<endl;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            cout<<setw(10)<<setprecision(4)<<fixed<<M[i][j]<<"  ";
        }
        cout<<endl;
    }
}

glm::mat4 translation(){
    // Constrindo uma Matriz identidade 
    glm::mat4 MIdentity = glm::identity<glm::mat4>();                           cout<<"Matriz Identity"<<endl;
    printFormat(MIdentity);


    vector<glm::vec3> figure3D = {
        glm::vec3{-1.0f, -1.0f, 0.0f},
        glm::vec3{ 1.0f, -1.0f, 0.0f},
        glm::vec3{ 0.0f,  1.0f, 0.0f},
    };



    // Construindo uma Matriz de translação
    glm::vec3 translationFactor {10.0f, 10.0f, 10.0f};                          cout<<"Fator de Translação: "<<glm::to_string(translationFactor)<<endl;
    glm::mat4 MTranslation = glm::translate(MIdentity, translationFactor);      cout<<"Matriz de Translação"<<endl;
    printFormat(MTranslation);

    glm::vec4 Position1 {10, 10, 10, 1};                                        cout<<"Position Origin: "<<glm::to_string(Position1)<<endl;
    glm::vec4 Direction {10, 10, 10, 0};

    glm::vec4 LinePosition = MTranslation * Position1;                          cout<<"Position1 Translated: "<<glm::to_string(LinePosition)<<endl;
    return MTranslation;
}

glm::mat4 scale(){

    // Constrindo uma Matriz identidade 
    glm::mat4 MIdentity = glm::identity<glm::mat4>();                           cout<<"Matriz Identity"<<endl;
    printFormat(MIdentity);

    // Construindo Matriz Escala
    glm::vec3 ScaleFactor {2, 2, 2};                                            cout<<"Fator de Escala: "<<glm::to_string(ScaleFactor)<<endl;
    glm::mat4 MScale = glm::scale(MIdentity, ScaleFactor);                      cout<<"Matriz de Escala"<<endl;
    printFormat(MScale);

    glm::vec4 Position1 {100, 100, 0, 0};                                       cout<<"Position1 Origin: "<<glm::to_string(Position1)<<endl;
    glm::vec4 Direction1 {100, 100, 0, 0};

    Position1 = MScale * Position1;                                             cout<<"Position Scale: "<<glm::to_string(Position1)<<endl;
    return MIdentity;
}

glm::mat4 rotation(){
    // Constrindo uma Matriz identidade 
    glm::mat4 MIdentity = glm::identity<glm::mat4>();                           cout<<"Matriz Identity"<<endl;
    printFormat(MIdentity);

    // Construindo uma Matriz de Escala
    const float RotationAngle = glm::radians(90.0f);                        // Rotaciona 90 Graus
    glm::vec3 RotationAxis {0, 0, 1};
    glm::mat4 MRotation = glm::rotate(MIdentity, RotationAngle, RotationAxis);
    printFormat(MRotation);

    glm::vec4 Position1 {100, 0, 0, 1};                                         cout<<"Posição Original: "<<glm::to_string(Position1)<<endl;
    glm::vec4 Direction1 {100, 0, 0, 0};

    Position1 = MRotation * Position1;                                          cout<<"Position1 rotated: "<<glm::to_string(Position1)<<endl;

    return MRotation;
}

glm::mat4  Compose(glm::mat4 MTranslation, glm::mat4 MScale, glm::mat4 MRotation){
    // Constrindo uma Matriz identidade 
    glm::mat4 MIdentity = glm::identity<glm::mat4>();                           cout<<"Matriz Identity"<<endl;
    printFormat(MIdentity);

    // Agrupamento das Transformações (Atenção na ordem)
    glm::mat4 MTransformation = MTranslation * MRotation * MScale;              cout<<"Matriz de Transformação"<<endl;
    printFormat(MTransformation);

    glm::vec4 Position {1,1,0,1};
    glm::vec4 Direction {1,1,0,0};

    Position = MTransformation * Position;
    Direction =  MTransformation * Direction;

    cout<<"Position: "<<glm::to_string(Position)<<endl;
    cout<<"Direction: "<<glm::to_string(Direction)<<endl;

    return MTransformation;
}


// Matriz de Projeção e visualização
glm::mat4  ModelViewProject() {
	//É a matriz formada pelas as transformações Transfalação, Rotação e Escala
	glm::mat4 MatrizModel = glm::identity<glm::mat4>();
	
	// Matriz de Visualização
	glm::vec3 Eye{ 0, 0, 4};       // Posição do Olho
	glm::vec3 Center{0, 0, 7};      // Para onde esta olhando
	glm::vec3 Up{6, 10, 0};          // Inclinação

	glm::mat4 MatrizView = glm::lookAt(Eye, Center, Up);                            cout << "Matriz de Visualização" << endl;
	printFormat(MatrizView);

	// Matriz de projeção
 	const float FoV = glm::radians(45.0f);          // Angulo de abertura da visão
	const float AspectioRate = 800.0f / 600.0f;     // Proporção da Tela Largura/Altura
	const float Near = 1.0f;                        // A menor distancia pra começar a ver as coisas
	const float Far = 100000.0f;                    // A maior distancia pra contiar a ver as coisas

	glm::mat4 MatrizProjecao = glm::perspective(FoV, AspectioRate, Near, Far);      cout << "Matriz de Projeção" << endl;
	printFormat(MatrizProjecao);

	// Gerando o modelo completo
	glm::mat4 ModelViewProjection = MatrizProjecao * MatrizView * MatrizModel;      cout << "Matriz MVP" << endl;
	printFormat(ModelViewProjection);

	// Aplicando o MVP em um ponto qualquer
	glm::vec4 Ponto{ 0,0,0,1 };                                                     cout << "Ponto Original: " << glm::to_string(Ponto) << endl;
	Ponto = ModelViewProjection * Ponto;                                            cout << "Ponto Projetado (Coordenada Homogenia): " << glm::to_string(Ponto) << endl;

    return ModelViewProjection;
}


int main(){
    setlocale(LC_ALL, "pt_BR");
    assert(glfwInit());                                                                         // Inicializando a biblioteca do GLFW
    GLFWwindow* window = glfwCreateWindow(Width, Height, "CG com OpenGL", nullptr, nullptr);    // Construindo a janela
    assert(window);                                                                             // Verificar se a janela é nula
    glfwMakeContextCurrent(window);      
    
    // inicializa o Glew
    int statusGlwInit = glewInit();
    assert(glewInit() == GLEW_OK);   


    // Define a matriz identidade
    glm::mat4 MatrizModel = glm::identity<glm::mat4>(); 

    // Define um objeto e realiza operaçoes nele
    glm::mat4 MTranslation        = translation();    
    glm::mat4 MScale               = scale();    
    glm::mat4 MRotation            = rotation();       
    glm::mat4 ResultMat            = Compose(MTranslation, MScale, MRotation);  
    glm::mat4 ModelViewProjection  = ModelViewProject();

    // Define uma figura
    vector<glm::vec3> figure3D = {
        glm::vec3{-1.0f, -1.0f, 0.0f},
        glm::vec3{ 1.0f, -1.0f, 0.0f},
        glm::vec3{ 0.0f,  1.0f, 0.0f},
    };
 
    // Aplica Ponto a ponto no ponto de visão
    for(glm::vec3& Vertice : figure3D){
        glm::vec4 VerticeProjected = ModelViewProjection * glm::vec4{Vertice, 1};

        // Calculando a coordenada homogenea
        VerticeProjected /= VerticeProjected.w;
        Vertice = VerticeProjected;
    }

    GLuint VertexBuffer;
    glGenBuffers(1, &VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(figure3D), figure3D.data(), GL_DYNAMIC_DRAW);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);                   // Limpar o Buffer de cor

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

        glVertexAttribPointer(0, figure3D.size(), GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_POLYGON, 0, figure3D.size());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(0);

        glfwPollEvents();                            // Processa todos os eventos na fila do GLFW
        glfwSwapBuffers(window);                     // Envia o conteúdo para desenhar na tela
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);


    return 0;
}