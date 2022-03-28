/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 05/03/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometryParede();
int setupGeometryTelhado();
int setupGeometryPorta();
int setupGeometryJanela0();
int setupGeometryJanela1();

const GLuint WIDTH = 800, HEIGHT = 600;

int main()
{
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mayara", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	Shader shader("../shaders/hello.vs", "../shaders/hello.fs");

	// Gerando um buffers com as geometrias da casa
	GLuint VAO_Parede = setupGeometryParede();
	GLuint VAO_Telhado = setupGeometryTelhado();
	GLuint VAO_Porta = setupGeometryPorta();
	GLuint VAO_Janela0 = setupGeometryJanela0();
	GLuint VAO_Janela1 = setupGeometryJanela1();

	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não está nos buffers
	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);

	glUseProgram(shader.ID);

	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);

		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glUseProgram(shader.ID);
		glBindVertexArray(VAO_Parede);
		glDrawArrays(GL_LINE_STRIP, 0, 4);

		glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
		glUseProgram(shader.ID);
		glBindVertexArray(VAO_Telhado);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glUseProgram(shader.ID);
		glBindVertexArray(VAO_Telhado);
		glDrawArrays(GL_LINE_LOOP, 0, 3);

		glUniform4f(colorLoc, 0.4f, 0.1f, 0.1f, 1.0f);
		glUseProgram(shader.ID);
		glBindVertexArray(VAO_Porta);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glUseProgram(shader.ID);
		glBindVertexArray(VAO_Porta);
		glDrawArrays(GL_LINE_STRIP, 0, 4);

		glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 1.0f);
		glUseProgram(shader.ID);
		glBindVertexArray(VAO_Janela0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glUseProgram(shader.ID);
		glBindVertexArray(VAO_Janela0);
		glDrawArrays(GL_LINE_LOOP, 0, 4);

		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glUseProgram(shader.ID);
		glBindVertexArray(VAO_Janela1);
		glDrawArrays(GL_LINE_STRIP, 0, 5);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO_Parede);
	glDeleteVertexArrays(1, &VAO_Telhado);
	glDeleteVertexArrays(1, &VAO_Porta);
	glDeleteVertexArrays(1, &VAO_Janela0);
	glDeleteVertexArrays(1, &VAO_Janela1);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometryParede()
{

	GLfloat vertices[] = {
		-0.6f, -1.0f, 0.0f,
		-0.6f,  0.2f, 0.0f,
		 0.6f,  0.2f, 0.0f ,
		 0.6f,  -1.0f, 0.0f,
	};

	GLuint VBO, VAO_Parede;


	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_Parede);

	glBindVertexArray(VAO_Parede);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO_Parede;
}

int setupGeometryTelhado()
{

	GLfloat vertices[] = {
		-0.6f, 0.2f, 0.0f,
		 0.0f, 1.0f, 0.0f,
		 0.6f, 0.2f, 0.0f ,
	};

	GLuint VBO, VAO_Telhado;


	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_Telhado);

	glBindVertexArray(VAO_Telhado);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO_Telhado;
}

int setupGeometryPorta()
{

	GLfloat vertices[] = {
		-0.1f, -1.0f, 0.0f,
		-0.1f,  -0.5f, 0.0f,
		 0.2f,  -0.5f, 0.0f ,
		 0.2f,  -1.0f, 0.0f,
	};

	GLuint VBO, VAO_Porta;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_Porta);

	glBindVertexArray(VAO_Porta);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO_Porta;
}

int setupGeometryJanela0()
{

	GLfloat vertices[] = {
		-0.4f, -0.4f, 0.0f,   
		-0.4f,  -0.0f, 0.0f, 
		 0.0f,  -0.0f, 0.0f , 
		 0.0f,  -0.4f, 0.0f,
	};

	GLuint VBO, VAO_Janela0;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_Janela0);

	glBindVertexArray(VAO_Janela0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);

	return VAO_Janela0;
}

int setupGeometryJanela1()
{

	GLfloat vertices[] = {
		-0.4f, -0.2f, 0.0f, 
		 0.0f,  -0.2f, 0.0f, 
		 0.0f,  0.0f, 0.0f , 
		-0.2f,  0.0f, 0.0f,
		-0.2f,  -0.4f, 0.0f
	};

	GLuint VBO, VAO_Janela1;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_Janela1);

	glBindVertexArray(VAO_Janela1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO_Janela1;
}

