/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico - Jogos Digitais - Unisinos
 * Vers�o inicial: 7/4/201
 * �ltima atualiza��o em 05/03/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Shader.h"

const float Pi = 3.1419;

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int generateCircle(float radius, int nPoints);

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;

//using namespace glm;

// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	//Muita aten��o aqui: alguns ambientes n�o aceitam essas configura��es
	//Voc� deve adaptar para a vers�o do OpenGL suportada por sua placa
	//Sugest�o: comente essas linhas de c�digo para desobrir a vers�o e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exerc�cio 1", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Compilando e buildando o programa de shader
	Shader shader("../shaders/ortho.vs", "../shaders/ortho.fs");

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	int nPoints = 100;
	GLuint VAO = generateCircle(0.5, nPoints);//setupGeometry();
	

	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a vari�veis do tipo uniform em GLSL para armazenar esse tipo de info
	// que n�o est� nos buffers
	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);
	
	glUseProgram(shader.ID);

	//Criando a matriz de proje��o usando a GLM
	glm::mat4 projection = glm::mat4(1); //matriz identidade
	projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));


	

	//shader.setMat4("model", glm::value_ptr(model));

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(5);
		glPointSize(5);

		glBindVertexArray(VAO);


		for (int i = 1; i < 4; i++) {
			glm::mat4 model = glm::mat4(1); // Matriz identidade
			model = glm::translate(model, glm::vec3(i * 150.0, i * 150.0, 0));
			model = glm::rotate(model, (float)5 / i * (float)glfwGetTime() /*  glm::radians(45.0f) */, glm::vec3(0, 0, 1));
			model = glm::scale(model, glm::vec3(i/5.0 , i/5.0 , 1.0));
			GLint modelLoc = glGetUniformLocation(shader.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

			glUniform4f(colorLoc, 1.0f, 0.0f, 1.0f, 1.0f);
			glDrawArrays(GL_TRIANGLE_FAN, 0, nPoints);

		}

		//Criando a matriz de modelo usando a GLM
		//glm::mat4 model = glm::mat4(1); //matriz identidade
		//model = glm::translate(model, glm::vec3(400.0, 300.0, 0));
		//model = glm::rotate(model, (float)glfwGetTime() /*glm::radians(90.0f)*/, glm::vec3(0, 0, 1));
		//model = glm::scale(model, glm::vec3(2.0, 2.0, 1.0));
		

		//glUniform4f(colorLoc, 1.0f, 0.0f, 1.0f, 1.0f); //enviando cor para vari�vel uniform inputColor
		//glDrawArrays(GL_POINTS, 0, nPoints+1);
		
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int generateCircle(float radius, int nPoints)
{
	int totalSize = (nPoints + 2) * 3;
	GLfloat* vertices = new GLfloat[totalSize];

	//Primeiro, colocamos o ponto central do c�rculo (x, y, z)
	vertices[0] = 0.0; //x
	vertices[1] = 0.0; //y
	vertices[2] = 0.0; //z

	float angle = 0.0;
	float slice = 2 * Pi / (GLfloat) nPoints;
	for (int i = 3; i < totalSize; i += 3)
	{
		float x = radius * cos(angle) * 100;
		float y = radius * sin(angle) * 100;
		float z = 0.0;

		vertices[i] = x;
		vertices[i + 1] = y;
		vertices[i + 2] = z;

		angle += slice;
	}

	GLuint VBO, VAO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, totalSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

