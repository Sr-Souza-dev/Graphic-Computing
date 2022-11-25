#include <bits/stdc++.h>
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
#include <glm/gtx/string_cast.hpp>

//Carregar a textura
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//Definindo as dimen��es da janela
const int Width = 800;
const int Heigth = 600;

//Fun��o responsavel por carregar os arquivos dos shaders
std::string LerArquivo(const char* caminho) {
	std::string conteudoArquivo;
	if (std::ifstream FileStream{ caminho , std::ios::in }) {
		conteudoArquivo.assign(std::istreambuf_iterator<char>(FileStream), std::istreambuf_iterator<char>());
	}
	return conteudoArquivo;
}

//Fun��o que validar se o shader carregou com sucesso
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

//Fun��o que carrega os Shaders
GLuint LoadShaders(const char* VertexShaderFile, const char* FragmentShaderFile) {
	//Carregando o c�digo fonte do shaders
	std::string VertexShaderSource = LerArquivo(VertexShaderFile);
	std::string FragmentShaderSource = LerArquivo(FragmentShaderFile);

	//Valida se carregou os arquivos corretamente
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

//Carregar a imagem da textura
GLint carregarTextura(const char* caminhoTextura) {
	std::cout << "Carregando textura: " << caminhoTextura << std::endl;

	//inverter a orienta��o da imagem
	stbi_set_flip_vertically_on_load(true);

	int LarguraImagem = 0;
	int AlturaImagem = 0;
	int NumeroDeComponentes = 0;
	unsigned char* DadosTextura = stbi_load(caminhoTextura, 
											&LarguraImagem, 
											&AlturaImagem, 
											&NumeroDeComponentes,
											3);
	assert(DadosTextura);

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

GLuint carregaGeometria() {
	//Defini��o dos verticies
	std::vector<glm::vec3>  colors = {
        glm::vec3{1.0f, 0.5f, 0.0f},  // 0. laranja
        glm::vec3{1.0f, 0.0f, 1.0f},  // 1. violeta
        glm::vec3{0.0f, 1.0f, 0.0f},  // 2. verde
        glm::vec3{0.0f, 0.0f, 1.0f},  // 3. azul
        glm::vec3{1.0f, 0.0f, 0.0f},  // 4. vermelho
        glm::vec3{1.0f, 1.0f, 0.0f},  // 5. amarelo
        glm::vec3{1.0f, 1.0f, 1.0f},  // 6. branco
        glm::vec3{0.8f, 0.8f, 0.8f},  // 7. cinza
    };
	float d = 1;
	std::array<Vertice, 8> Quadrado = {
		//FRONT
        Vertice{glm::vec3{-d,  d,  d}, colors[0], glm::vec2{0.0f,1.0f}},
        Vertice{glm::vec3{-d, -d,  d}, colors[1], glm::vec2{0.0f,1.0f}},
        Vertice{glm::vec3{ d,  d,  d}, colors[2], glm::vec2{0.0f,1.0f}},
        Vertice{glm::vec3{ d, -d,  d}, colors[3], glm::vec2{0.0f,1.0f}},

        //BACK
        Vertice{glm::vec3{ d,  d,  -d}, colors[4], glm::vec2{0.0f,1.0f}},
        Vertice{glm::vec3{ d, -d,  -d}, colors[5], glm::vec2{0.0f,1.0f}},
        Vertice{glm::vec3{-d,  d,  -d}, colors[6], glm::vec2{0.0f,1.0f}},
        Vertice{glm::vec3{-d, -d,  -d}, colors[4], glm::vec2{0.0f,1.0f}}
	};

	//Defini��o do array de elementos que formam os trinagulos
	std::array<glm::ivec3, 12> Indices{
		// FRONT
        glm::ivec3{	0,  1,	2},
        glm::ivec3{	2,  3,	1},

        // LEFT
        glm::ivec3{	1,  7,	0},
        glm::ivec3{	0,  6,	7},

        //BACK
        glm::ivec3{	7,  6,	5},
        glm::ivec3{	5,  4,	6},

        // UP
        glm::ivec3{	6,  4,	0},
        glm::ivec3{	0,  2,	4},
        
        // RIGHT
        glm::ivec3{	4,  5,	2},
        glm::ivec3{	2,  3,	5},

        // DOWN
        glm::ivec3{	5,  7,	3},
        glm::ivec3{	3,  1,	7}
	};

	// Vari�vel que vai conter o identificador do buffer de v�rtices
	GLuint VertexBuffer;

	// Gerar um identificador glGenBuffers(NUMERO DE BUFFER, INDETIFICADOR);
	glGenBuffers(1, &VertexBuffer);

	//Gerar a identifica��o do VertexBuffer (VBO) com o OpenGL 
	GLuint ElementBuffer = 0;
	glGenBuffers(1, &ElementBuffer);

	//Ativar o buffer de v�rtices, comandos v�o afetar o VertexBuffer.
	//O buffer para onde vamos copiar os vertices do triangulo.
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	// Passa para o OpenGL o ponteiro para os dados que ser�o copiados para GPU
	// basicamente copia os dados para a mem�ria de v�deo
	glBufferData(GL_ARRAY_BUFFER, sizeof(Quadrado), Quadrado.data(), GL_STATIC_DRAW);

	//Copiar os dados para o bufffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices.data(), GL_STATIC_DRAW);

	//Gerar o Vertex Arrays Object VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// agora vamos habilitar o VAO
	glBindVertexArray(VAO);

	// Habilita as posi��es
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// glBindBuffer(TIPO, BUFFER CRIADO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	// Definindo os atributos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, Cor)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertice),
		reinterpret_cast<void*>(offsetof(Vertice, UV)));

	//Desabilita o VAO
	glBindVertexArray(0);

	return VAO;
}

class FlyCamera {
public:

	void MoverPraFrente(float quantidade) {
		this->Location += glm::normalize(Direction) * quantidade * Speed;
	}

	void MoverPraDireita(float quantidade) {
		//Calcula o produto vetorial
		glm::vec3 Right = glm::normalize(glm::cross(Direction, Up));

		this->Location += Right * quantidade * Speed;
	}

	void OlharPara(float quantidadeEixoYyaw, float quantidadeEixoXpitch) {
		// Aplicando a velocidade do movimento
		quantidadeEixoYyaw *= Sensitivity;
		quantidadeEixoXpitch *= Sensitivity;
		
		// Matriz identidade
		const glm::mat4 MatrizIdentidade = glm::identity<glm::mat4>();
		// Produto vetorial
		glm::vec3 Right = glm::normalize(glm::cross(Direction, Up));
		
		// Criando as matrizes de rota��o
		glm::mat4 MatrizRotacaoExioY = glm::rotate(MatrizIdentidade,
										glm::radians(quantidadeEixoYyaw),
									Up);
		glm::mat4 MatrizRotacaoExioX = glm::rotate(MatrizIdentidade,
									glm::radians(quantidadeEixoXpitch),
								Right);

		// Plicando nos vetores de dire��o
		this->Up = MatrizRotacaoExioX * glm::vec4{ this->Up , 0.0f };
		this->Direction = MatrizRotacaoExioY * MatrizRotacaoExioX * glm::vec4{ this->Direction , 0.0f };
	}

	glm::mat4 GetViewProjection() const {

		glm::mat4 View = glm::lookAt(Location, Location + Direction, Up);
		glm::mat4 Projection = glm::perspective(FieldOfView, AspectRatio, Near, Far);
		return Projection * View;
	}
	
	//Parametros de Interatividade, define a velocidade do movimento
	float Speed = 1;
	float Sensitivity = 0.1f;

	//Parametros da Matriz de view
	glm::vec3 Location{	0.0f, 0.0f, 5.0f };
	glm::vec3 Direction{ 0.0f, 0.0f, -1.0f };
	glm::vec3 Up{ 0.0f, 1.0f, 0.0f };

	//Par�metros da Matriz Projection
	float FieldOfView = glm::radians(45.0f);
	float AspectRatio = Width / Heigth;
	float Near = 0.01f;
	float Far = 1000.0f;
};

FlyCamera Camera;

glm::vec2 PosicaoAnteriorCursor;
bool habilitarMovimentoMouse = false;

// Fun��es de callback do mouse
//Fun��o de callback, tem que ter estrutura padr�o
void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Modifiers){
	//Essa fun��o retorna quais botoes do mouse foram clicados.
	//Verifica se � bot�o esquerdo do mouse
	if (Button == GLFW_MOUSE_BUTTON_LEFT){	
		//Verifica qual a a��o relacizada com o bot�o esquerdo
		if (Action == GLFW_PRESS){
			//Desabilita o curso quando estiver fora da window
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			double X, Y;
			//Retorna a posi��o do mouse na tela
			glfwGetCursorPos(Window, &X, &Y);
			PosicaoAnteriorCursor = glm::vec2{ X, Y };
			habilitarMovimentoMouse = true;
		}
		// Verifica se a a��o foi encerrada
		else if (Action == GLFW_RELEASE){
			//Habilita novamente o cursos, para ter o funcionamento normal
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			habilitarMovimentoMouse = false;
		}
	}
}

//Fun��o de callback, tem que ter estrutura padr�o
void MouseMotionCallback(GLFWwindow* Window, double X, double Y) {
	//Essa fun��o retorna a posi��o do mouse na tela

	if (habilitarMovimentoMouse){
		glm::vec2 PosicaoAtualCursor{ X, Y };
		glm::vec2 Delta = PosicaoAnteriorCursor - PosicaoAtualCursor;

		Camera.OlharPara(-Delta.x, -Delta.y);

		PosicaoAnteriorCursor = PosicaoAtualCursor;
	}
}

/*
void ResizeCallback(GLFWwindow* Window, int Width, int Height)
{
	Camera.AspectRatio = static_cast<float>(Width) / Height;
	glViewport(0, 0, Width, Height);
}
*/

int main() {
	setlocale(LC_ALL, "pt_BR");
	//Inicializando a biblioteca do GLFW 
	assert(glfwInit() == GLFW_TRUE);

	//Construindo a janela
	GLFWwindow* window = glfwCreateWindow(Width, Heigth, "CG com OpenGL", nullptr, nullptr);
	//Verificar se a janela � nula
	assert(window);

	//Cadastrar callbacks no GLFW
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, MouseMotionCallback);

	//Definindo a janela de contexto
	glfwMakeContextCurrent(window);

	//Desabilita a sircroniza��o de FPS entre o processamenro e monitor
	//glfwSwapInterval(1);

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
	//GLuint TexturaId = carregarTextura("textures/mundo.jpg");

	//Carregando a geometria
	GLuint QuadVAO = carregaGeometria();

	std::cout<<"Aqui>> "<<ProgramaId<<std::endl;

	//Criando o Model
	glm::mat4 MatrizModel = glm::identity<glm::mat4>();

	//Definir cor de fundo
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	//Tempo de atualizado dos frames
	double  TempoAtualizadoFrameAnterior = glfwGetTime();

	//Habilitar o Backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Construindo o loop de eventos
	while (!glfwWindowShouldClose(window)) {
		//Calculando o DeltaTime
		double  TempoAtualizadoFrameAtual = glfwGetTime();
		double DeltaTime = TempoAtualizadoFrameAtual - TempoAtualizadoFrameAnterior;
		if (DeltaTime > 0) {
			TempoAtualizadoFrameAnterior = TempoAtualizadoFrameAtual;
		}
		// Limpar o Buffer de cor
		glClear(GL_COLOR_BUFFER_BIT);

		// Ativar o Shader criado
		glUseProgram(ProgramaId);
		
		// Calculando a MVP dinamicamente
		glm::mat4 MatrizViewProjection = Camera.GetViewProjection();
		glm::mat4 ModelViewProjection = MatrizViewProjection * MatrizModel;

		//Obter a localiza��o da MPV
		GLint ModelViewProjectionLoc = glGetUniformLocation(ProgramaId, "ModelViewProjection");
		//Definindo os valores do Uniform
		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

		//Habilitando a Textura no indice 0
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, TexturaId);

		// GLint TextureSamplerLoc = glGetUniformLocation(ProgramaId, "TextureSampler");
		// glUniform1i(TextureSamplerLoc, 0);

		//Passando o objeto que ser� desenhado
		glBindVertexArray(QuadVAO);

		// Definir o tamanho dos pontos a serem desenhados
		glPointSize(10.0f);
		//Definindo a largura da linha
		glLineWidth(10.0f);

		//Definindo o modo de desenho
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//Desenhar os pontos
		// glDrawArrays(GL_TRIANGLES, 0, Quadrado.size());
		glDrawElements(GL_TRIANGLES, 12*6,GL_UNSIGNED_INT, nullptr);

		//Desabilitando o glBindVertexArray(QuadVAO), pq j� desenhou
		glBindVertexArray(0);

		//Desabilita o programa
		glUseProgram(0);

		glfwPollEvents();
		glfwSwapBuffers(window);

		// Capturando eventor do teclado
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			Camera.MoverPraFrente(1.0f * DeltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			Camera.MoverPraFrente(-1.0f * DeltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			Camera.MoverPraDireita(1.0f * DeltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			Camera.MoverPraDireita(-1.0f * DeltaTime);
		}
	}

	glDeleteVertexArrays(1, &QuadVAO);

	//Finaliza a biblioteca GLFW
	glfwTerminate();
	return 0;
}
