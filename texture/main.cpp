#include <iostream>
//Biblioteca do OpenGL
#include <GL/glew.h>
//Biblioteca que ir� desenhar a tela
#include <GLFW/glfw3.h>
//Biliotecas de valida��o
#include <cassert>
#include <array>
//Biblioteca para manipula��o de arquivos
#include <fstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

//Carregar a textura
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image.h>

//Definindo as dimen��es da janela
const int Width = 800;
const int Heigth = 600;

//Fun��o responsavel por carregar os shaders
std::string LerArquivo(const char* caminho) {
	std::string conteudoArquivo;
	if (std::ifstream FileStream{ caminho , std::ios::in }) {
		conteudoArquivo.assign(std::istreambuf_iterator<char>(FileStream), std::istreambuf_iterator<char>());
	}
	return conteudoArquivo;
}


void CheckShader(GLuint ShaderId) {
	GLint Result = GL_TRUE;

	glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Result);

	if (Result == GL_FALSE) {
		GLint tamanhoLog = 0;
		glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &tamanhoLog);
		
		if (tamanhoLog > 0) {
			std::string textoLog(tamanhoLog, '\0');
			glGetShaderInfoLog(ShaderId, tamanhoLog, nullptr, &textoLog[0]);

			std::cout << "Erro no shader:" << std::endl;
			std::cout << textoLog << std::endl;

			assert(false);
		}
		
	}
}

GLuint LoadShaders(const char* VertexShaderFile, const char* FragmentShaderFile) {

	//Carregando o c�digo fonte do shaders
	std::string VertexShaderSource = LerArquivo(VertexShaderFile);
	std::string FragmentShaderSource = LerArquivo(FragmentShaderFile);

	assert(!VertexShaderSource.empty());
	assert(!FragmentShaderSource.empty());

	//Criando os identificadores do shaders
	GLuint VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	//Compilando os shaders
	const char* VertexShaderSourcePtr = VertexShaderSource.c_str();
	glShaderSource(VertexShaderId, 1, &VertexShaderSourcePtr, nullptr);
	glCompileShader(VertexShaderId);
	// verifica se a compila��o deu certo
	CheckShader(VertexShaderId);

	//Compilando os shaders
	const char* FragmentShaderPtr = FragmentShaderSource.c_str();
	glShaderSource(FragmentShaderId, 1, &FragmentShaderPtr, nullptr);
	glCompileShader(FragmentShaderId);
	// verifica se a compila��o deu certo
	CheckShader(FragmentShaderId);
	

	// Gerar o programa
	GLuint ProgramaId = glCreateProgram();
	glAttachShader(ProgramaId, VertexShaderId);
	glAttachShader(ProgramaId, FragmentShaderId);
	glLinkProgram(ProgramaId);

	//Verificar se o programa foi constru�do corretamente
	GLint Result = GL_TRUE;
	glGetProgramiv(ProgramaId, GL_LINK_STATUS, &Result);

	if (Result == GL_FALSE) {
		// Obter o motivo de n�o funcionar
		GLint tamanhoLog = 0;
		glGetProgramiv(ProgramaId, GL_INFO_LOG_LENGTH, &tamanhoLog);

		if (tamanhoLog > 0) {
			std::string textoLog(tamanhoLog, '\0');
			glGetProgramInfoLog(ProgramaId, tamanhoLog, nullptr, &textoLog[0]);

			std::cout << "Erro no Program:" << std::endl;
			std::cout << textoLog << std::endl;

			assert(false);
		}
	}

	//Excluindo tudo
	glDetachShader(ProgramaId, VertexShaderId);
	glDetachShader(ProgramaId, FragmentShaderId);

	glDeleteShader(VertexShaderId);
	glDeleteShader(FragmentShaderId);

	return ProgramaId;
}

GLint carregarTextura(const char* caminhoTextura) {
	std::cout << "Carregando textura: " << caminhoTextura << std::endl;

	stbi_set_flip_vertically_on_load(true);

	int LarguraImagem = 0;
	int AlturaImagem = 0;
	int NumeroDeComponentes = 0;
	unsigned char* DadosTextura = stbi_load("pedra.jpg", 
											&LarguraImagem, 
											&AlturaImagem, 
											&NumeroDeComponentes,
											0);
	if(DadosTextura == NULL) {
		printf("Error in loading the image\n");
		exit(1);
	}

	//Vamos gerar o identificador da textura
	GLuint TexturaId;
	glGenTextures(1, &TexturaId);

	//Habilitar a textura
	glBindTexture(GL_TEXTURE_2D, TexturaId);

	//Copiar a textura da mem�ria RAM pra mem�ria de v�deo
	glTexImage2D(GL_TEXTURE_2D, 
		0, //Level
		GL_RGB,//Formato da imagem
		LarguraImagem,
		AlturaImagem,
		0, //Borda da imagem
		GL_RGB,//Formato de Pixel dentro do Shader
		GL_UNSIGNED_BYTE,//Formato dos dados
		DadosTextura);

	//Definindo o filtro que ser� aplicado, magnifica��o, aproxima da tela
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Definindo o filtro de minimifica��o, quando afasta da tela
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//Configurando o Wrapping, o que vai acontecer com a textura fora das coordenadas
	// Na coordenada X = U = S => GL_TEXTURE_WRAP_S
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// Na coordenada Y = V = T => GL_TEXTURE_WRAP_T
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Gerar o mipmap com base na textura
	glGenerateMipmap(GL_TEXTURE_2D);

	//Desligar a textura
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(DadosTextura);

	return TexturaId;
}

struct Vertice {
	glm::vec3 Posicao;
	glm::vec3 Cor;
	glm::vec2 UV;
};

int main() {
	setlocale(LC_ALL, "pt_BR");
	//Inicializando a biblioteca do GLFW 
	assert(glfwInit() == GLFW_TRUE);

	//Construindo a janela
	GLFWwindow* window = glfwCreateWindow(Width, Heigth, "CG com OpenGL", nullptr, nullptr);
	//Verificar se a janela � nula
	assert(window);

	//Definindo a janela de contexto
	glfwMakeContextCurrent(window);

	//inicializa o GLEW
	int statusGlwInit = glewInit();
	std::cout << statusGlwInit << std::endl;
	assert(glewInit() == GLEW_OK);
	
	//Verificar a vers�o do OpenGL que est� sendo usando
	GLint GLMajorVersion = 0;
	GLint GLMinorVersion = 0;

	glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);

	std::cout << "Maior vers�o do OpenGL suportada:" << GLMajorVersion << std::endl;
	std::cout << "Menor vers�o do OpenGL suportada:" << GLMinorVersion << std::endl;

	//Obtendo informa��es do driver utilizado
	std::cout << "Fabircante do driver de video:" << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Modelo da placa de video:" << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Vers�o do OpenGL:" << glGetString(GL_VERSION) << std::endl;
	std::cout << "Vers�o do GLS:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	//Carregando os Shaders
	GLuint ProgramaId = LoadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");

	//Carregando texturas
	GLuint TexturaId = carregarTextura("./textures/pedra.jpg");

	//PSSO 1: Definir um tri�ngulo em coordenadas normalizadas
	std::array<Vertice, 3> Triangulo = {
		Vertice{
			glm::vec3{-1.0f, -1.0f, 0.0f},
			glm::vec3{1.0f, 0.0f, 0.0f}, 
			glm::vec2{0.0f, 0.0f} 
		},
		Vertice{
			glm::vec3{ 1.0f, -1.0f, 0.0f},
			glm::vec3{0.0f, 1.0f, 0.0f},
			glm::vec2{1.0f, 0.0f}
		},
		Vertice{
			glm::vec3{ 0.0f,  1.0f, 0.0f},
			glm::vec3{0.0f, 0.0f, 1.0f},
			glm::vec2{0.5f, 1.0f}
		},
	};

	//Aplicando o MVP
	// Criando o Model
	glm::mat4 MatrizModel = glm::identity<glm::mat4>();

	// Criando a View
	glm::vec3 Eye{ 0, 10, 10 };
	glm::vec3 Center{ 0, 0, 0 };
	glm::vec3 Up{ 0, 1, 0 };
	glm::mat4 MatrizView = glm::lookAt(Eye, Center, Up);

	//Criando a Projection
	const float FoV = glm::radians(10.0f);//Semelhante ao ZOOM
	const float AspectioRate = Width / Heigth;
	const float Near = 0.01f;
	const float Far = 1000.0f; 
	glm::mat4 MatrizProjection = glm::perspective(FoV, AspectioRate, Near, Far);

	// Gerando o ModelViewProjection
	// Usando a CPU para calculo, mas o ideal � usar a GPU
	glm::mat4 ModelViewProjection = MatrizProjection * MatrizView * MatrizModel;

	//Todos esse calculo ser� realizado na GPU
	//Aplicando o MVP no triangulo
	/*
	for (Vertice& Vertice : Triangulo) {
		glm::vec4 VerticeProjetado = ModelViewProjection * glm::vec4{ Vertice.Posicao , 1 };

		// calculando a coordenada homogena 
		VerticeProjetado /= VerticeProjetado.w;
		Vertice.Posicao = VerticeProjetado;
	}*/
	//FIM - PSSO 1
	
	//PASSO 2: copiar os v�rtices do tri�ngulo para a mem�ria da GPU
	// Vari�vel que vai conter o identificador do buffer de v�rtices
	GLuint VertexBuffer;

	// Gerar um identificador glGenBuffers(NUMERO DE BUFFER, INDETIFICADOR);
	glGenBuffers(1, &VertexBuffer);

	//Ativar o buffer de v�rtices, comandos v�o afetar o VertexBuffer.
	//O buffer para onde vamos copiar os vertices do triangulo.
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	// Passa para o OpenGL o ponteiro para os dados que ser�o copiados para GPU
	// basicamente copia os dados para a mem�ria de v�deo
	// glBufferData(ORIGEM DOS DADOS, TAMANHO EM BYTES, PONTEIRO PARA OS DADOS
	//				, TIPO DE USO DO BUFFER);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangulo), Triangulo.data(), GL_STATIC_DRAW);
	//PASSO 2 - FIM

	//Definir cor de fundo
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	//Construindo o loop de eventos
	while (!glfwWindowShouldClose(window)) {
		// Limpar o Buffer de cor
		glClear(GL_COLOR_BUFFER_BIT);

		// Ativar o Shader criado
		glUseProgram(ProgramaId);
		
		//Obter a localiza��o da MPV
		GLint ModelViewProjectionLoc = glGetUniformLocation(ProgramaId, "ModelViewProjection");
		//Definindo os valores do Uniform
		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));
		

		//Habilitando a Textura no indice 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexturaId);

		GLint TextureSamplerLoc = glGetUniformLocation(ProgramaId, "TextureSampler");
		glUniform1i(TextureSamplerLoc, 0);

		// Habilita a posi��o 0
		glEnableVertexAttribArray(0);
		// habilita a cor na posi��o 1
		glEnableVertexAttribArray(1);
		// habilita a cor na posi��o 2
		glEnableVertexAttribArray(2);

		// glBindBuffer(TIPO, BUFFER CRIADO);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		
		// Definindo o atributo 0
		glVertexAttribPointer(0, Triangulo.size(), GL_FLOAT, GL_FALSE, sizeof(Vertice), nullptr);
		// Definindo o atributo 1
		glVertexAttribPointer(1, Triangulo.size(), GL_FLOAT, GL_TRUE, sizeof(Vertice), 
		 	reinterpret_cast<void*>(offsetof(Vertice,Cor)));
		// Definindo o atributo 2
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertice),
			reinterpret_cast<void*>(offsetof(Vertice, UV)));

		
		// glDrawArrays(QUE OBJETO SER� DESENHADO, INDICE DO OBJETO
		//				, QUANTIDADE DE VERTICES PRA DESENHAR);
		glDrawArrays(GL_TRIANGLES, 0, Triangulo.size());
		
		//Desliga o glBindBuffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glUseProgram(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//Desalocar o VertexBuffer da mem�ria de video
	glDeleteBuffers(1, &VertexBuffer);

	//Finaliza a biblioteca GLFW
	glfwTerminate();
	return 0;
}
