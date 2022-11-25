#include "../headers/figure.hpp"

void Figure::setVectorSorting(std::vector<int> vec){
    int cont = 0, pos = -(vec.size() * 3)/2, max = 0, idx = 0;
    this->vec = vec;
    for(auto item : vec){
        if(item > max) max = item;
        form.vertices.push_back (Vertex{glm::vec3{pos,     -2, 1}, colors[cont % colors.size()]});
        form.vertices.push_back (Vertex{glm::vec3{pos+2,   -2, 1}, colors[cont % colors.size()]});
        form.vertices.push_back (Vertex{glm::vec3{pos,   item, 1}, colors[cont % colors.size()]});
        form.vertices.push_back (Vertex{glm::vec3{pos+2, item, 1}, colors[cont % colors.size()]});

        form.vertices.push_back (Vertex{glm::vec3{pos,     -2, 0}, colors[cont % colors.size()]});
        form.vertices.push_back (Vertex{glm::vec3{pos+2,   -2, 0}, colors[cont % colors.size()]});
        form.vertices.push_back (Vertex{glm::vec3{pos,   item, 0}, colors[cont % colors.size()]});
        form.vertices.push_back (Vertex{glm::vec3{pos+2, item, 0}, colors[cont % colors.size()]});
        cont++; 

        form.order.push_back (glm::ivec3{idx+0, idx+1, idx+2});
        form.order.push_back (glm::ivec3{idx+2, idx+3, idx+0});

        form.order.push_back (glm::ivec3{idx+3, idx+0, idx+6});
        form.order.push_back (glm::ivec3{idx+6, idx+4, idx+3});

        form.order.push_back (glm::ivec3{idx+4, idx+6, idx+7});
        form.order.push_back (glm::ivec3{idx+7, idx+5, idx+4});

        form.order.push_back (glm::ivec3{idx+2, idx+5, idx+7});
        form.order.push_back (glm::ivec3{idx+7, idx+1, idx+2});

        form.order.push_back (glm::ivec3{idx+3, idx+4, idx+5});
        form.order.push_back (glm::ivec3{idx+5, idx+2, idx+3});

        form.order.push_back (glm::ivec3{idx+0, idx+1, idx+7});
        form.order.push_back (glm::ivec3{idx+7, idx+6, idx+0});

        pos += 3; idx +=8;
    }
    normalizeVertices(pos-1, max);
    loadFigure();
}

std::vector<std::pair<float,float>> Figure::sortByStep(int interaction){
    int min = vec[interaction], pos = interaction;
    for(int i = interaction; i < vec.size(); i++){
        if(vec[i] < min){
            min = vec[i]; 
            pos = i;
        }
    }

    int aux   = vec[pos];
    vec[pos]  = vec[interaction];
    vec[interaction] = aux;

    interaction *= 8;   pos *= 8;
    return{
        {interaction, pos},
        {form.vertices[interaction].position[0], form.vertices[interaction+1].position[0]},
        {form.vertices[pos].position[0],         form.vertices[pos+1].position[0]}
    };
}

void Figure::swapFig(int first, int second, float step){
    int cont = 8;
    while(cont--){
        form.vertices[first].position[0]  = form.vertices[first].position[0]  + step;
        form.vertices[second].position[0] = form.vertices[second].position[0] - step;
        first++; second++;
    }
}

void Figure::swapVertex(int first, int second){
    int cont = 8;
    glm::vec3 aux;
    while(cont--){
        aux = form.vertices[first].position;
        form.vertices[first].position = form.vertices[second].position;
        form.vertices[second].position = aux;
        first++; second++;
    }
}

void Figure::printVec(){
    for(auto item:vec){
        std::cout<<item<<" | ";
    } std::cout<<std::endl;
}

void Figure::normalizeVertices(int maxWidth, int maxHeight){
    int mult = 2;
    for(auto& item:form.vertices){
        item.position = {mult*item.position[0]/maxWidth, (mult*item.position[1] - maxHeight/2)/maxHeight, mult*item.position[2]/maxHeight};
    }
}



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

    // std::cout<<"\n"<<"*************************************"<<" Ordem "<<"*************************************"<<"*"<<std::endl;
    // for(auto position : form.order){
    //     std::cout<<"Position ->  N1: "<<position[0]<<" | N2: "<<position[1]<<" | N3: "<<position[2]<<std::endl;
    // }
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