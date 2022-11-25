#include "../headers/figure.hpp"

void Figure::setCube(float d){
    form.vertices = {
		//FRONT
        Vertex{glm::vec3{-d,  d,  d}, colors[0]},
        Vertex{glm::vec3{-d, -d,  d}, colors[1]},
        Vertex{glm::vec3{ d,  d,  d}, colors[2]},
        Vertex{glm::vec3{ d, -d,  d}, colors[3]},

        //BACK
        Vertex{glm::vec3{ d,  d,  -d}, colors[4]},
        Vertex{glm::vec3{ d, -d,  -d}, colors[5]},
        Vertex{glm::vec3{-d,  d,  -d}, colors[6]},
        Vertex{glm::vec3{-d, -d,  -d}, colors[4]}
	};

	form.order = {
		// FRONT
        glm::ivec3{	1,  0,	2},
        glm::ivec3{	2,  3,	1},

        // LEFT
        glm::ivec3{	0,  6,	4},
        glm::ivec3{	4,  2,	0},

        //BACK
        glm::ivec3{	4,  5,	3},
        glm::ivec3{	3,  2,	4},

        // UP
        glm::ivec3{	4,  6,	7},
        glm::ivec3{	7,  5,	4},
        
        // RIGHT
        glm::ivec3{	7,  6,	0},
        glm::ivec3{	0,  1,	7},

        // DOWN
        glm::ivec3{	7,  5,	3},
        glm::ivec3{	3,  1,	7}
	};
    loadFigure();
}

void Figure::setPyramid (float d){
    form.vertices = {
        Vertex{glm::vec3{0,  2*d,  0}, colors[0]},    // 1

        Vertex{glm::vec3{-d,  -d, -d}, colors[1]},   // 2
        Vertex{glm::vec3{-d,  -d,  d}, colors[2]},   // 3
        Vertex{glm::vec3{ d,  -d, -d}, colors[4]},   // 4
        Vertex{glm::vec3{ d,  -d,  d}, colors[3]},   // 5
	};

	form.order = {
        glm::ivec3{	0,  2,	3},
        glm::ivec3{	4,	1,  0},
        glm::ivec3{	3,	0,  4},
        glm::ivec3{	0,  1,	2},

        glm::ivec3{	4,  3,	1},
        glm::ivec3{	1,  2,	4}
	};
    loadFigure();
}

Fig  Figure::getFig (){return this->form;}

void Figure::loadFigure(){
	GLuint VertexBuffer;        	                // Variavel que vai conter o identificador do buffer de vertices
	glGenBuffers(1, &VertexBuffer);                 // Gerar um identificador glGenBuffers(NUMERO DE BUFFER, INDETIFICADOR);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);    // O buffer para onde vamos copiar os vertices do triangulo.

    //Gerar a identificao do VertexBuffer (VBO) com o OpenGL 
	GLuint ElementBuffer = 0;
	glGenBuffers(1, &ElementBuffer);

	// Passa para o OpenGL o ponteiro para os dados que ser�o copiados para GPU
	// basicamente copia os dados para a mem�ria de v�deo
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*form.vertices.size(), form.vertices.data(), GL_STATIC_DRAW);

	//Copiar os dados para o bufffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*form.order.size()*3, form.order.data(), GL_STATIC_DRAW);

	//Gerar o Vertex Arrays Object VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// agora vamos habilitar o VAO
	glBindVertexArray(VAO);

	// Habilita as posi��es
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// glBindBuffer(TIPO, BUFFER CRIADO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	// Definindo os atributos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex, color)));

	//Desabilita o VAO
	glBindVertexArray(0);

    this->VAO = VAO;
}


void Figure::printd(){
    std::cout<<"*************************************"<<" Vertices "<<"*************************************"<<std::endl;
    for(auto vertex : form.vertices){
        std::cout<<"Position ->  X: "<<std::setw(5)<<vertex.position[0]<<" | Y: "<<std::setw(5)<<vertex.position[1]<<" | Z: "<<std::setw(5)<<vertex.position[2]<<std::endl;
        std::cout<<"Color    ->  R: "<<std::setw(5)<<vertex.color[0]<<" | G: "<<std::setw(5)<<vertex.color[1]<<" | B: "<<std::setw(5)<<vertex.color[2]<<std::endl<<std::endl;
    }

    std::cout<<"\n"<<"*************************************"<<" Ordem "<<"*************************************"<<"*"<<std::endl;
    for(auto position : form.order){
        std::cout<<"Position ->  N1: "<<position[0]<<" | N2: "<<position[1]<<" | N3: "<<position[2]<<std::endl;
    }
}


void Figure::translation (glm::vec3 factor){
    glm::mat4 identity      = glm::identity<glm::mat4>();
    glm::mat4 mTranslation  = glm::translate(identity, factor);

    for(auto& vertex : form.vertices){
        vertex.position = glm::vec3(mTranslation * glm::vec4({vertex.position, 1.0f}));
    }
}

void Figure::scale (glm::vec3 factor){
    glm::mat4 identity  = glm::identity<glm::mat4>();
    glm::mat4 mScale    = glm::scale(identity, factor);

    for(auto& vertex : form.vertices){
        vertex.position = glm::vec3(mScale * glm::vec4({vertex.position, 1.0f}));
    }
}

void Figure::rotation (glm::vec3 axis, float angle){
    glm::mat4 identity   = glm::identity<glm::mat4>();
    glm::mat4 mRotation  = glm::rotate(identity, glm::radians(angle), axis);

    for(auto& vertex : form.vertices){
        vertex.position = glm::vec3(mRotation * glm::vec4({vertex.position, 1.0f}));
    }
}

void Figure::reflection (glm::vec3 axis){
    for(auto& vertex : form.vertices){
        vertex.position = vertex.position * axis;
    }
} 


/*
    form.order = {
		// FRONT
        glm::ivec3{	0,  1,	2},
        glm::ivec3{	2,  3,	1},

        // // LEFT
        // glm::ivec3{	1,  7,	0},
        // glm::ivec3{	0,  6,	7},

        // //BACK
        // glm::ivec3{	7,  6,	5},
        // glm::ivec3{	5,  4,	6},

        // // UP
        // glm::ivec3{	6,  4,	0},
        // glm::ivec3{	0,  2,	4},
        
        // // RIGHT
        // glm::ivec3{	4,  5,	2},
        // glm::ivec3{	2,  3,	5},

        // // DOWN
        // glm::ivec3{	5,  7,	3},
        // glm::ivec3{	3,  1,	7}
	};

*/