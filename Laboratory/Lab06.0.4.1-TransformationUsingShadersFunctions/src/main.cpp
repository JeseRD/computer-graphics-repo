//============================================================================
// Name        : Trasnformation Functions with Shaders
// Professor   : Herminio Paucar
// Version     :
// Description : Display Transformation(Translate, Rotation and Scale) using only Shaders
//============================================================================

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>

#include "Utils.h"

GLuint renderingProgram;
GLfloat* m_Vertices;
GLuint n_Vertices;
GLuint m_VBO;
GLuint m_VAO;

// location of triangle on x axis
float x = 0.0f;
// offset for moving the triangle
float inc = 0.01f;

using namespace std;

void init () {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");

	// Cria um ID na GPU para um array de  buffers
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

    // The first 3 points are to Vertex position of Triangle
	m_Vertices = new GLfloat[9] {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};
	n_Vertices = 9;
	// Cria um ID na GPU para nosso buffer
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// Reserva memoria na GPU para um TARGET receber dados
	// Copia esses dados pra essa �rea de memoria
	glBufferData(
			GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
			n_Vertices * sizeof(GLfloat),	// tamanho do buffer
			m_Vertices,			// Dados a serem copiados pra GPU
			GL_STATIC_DRAW		// Pol�tica de acesso aos dados, para otimiza��o
		);

	glVertexAttribPointer(
			0,			// Lembra do (layout = 0 ) no vertex shader ? Esse valor indica qual atributo estamos indicando
			3,			// cada vertice � composto de 3 valores
			GL_FLOAT,	// cada valor do v�rtice � do tipo GLfloat
			GL_FALSE,	// Quer normalizar os dados e converter tudo pra NDC ? ( no nosso caso, j� esta tudo correto, ent�o deixamos como FALSE)
			0 * sizeof(GLfloat),// De quantos em quantos bytes, este atributo � encontrado no buffer ? No nosso caso 3 floats pros vertices + 3 floats pra cor = 6 floats
			(GLvoid*) 0	// Onde est� o primeiro valor deste atributo no buffer. Nesse caso, est� no in�cio do buffer
		);
	glEnableVertexAttribArray(0);	// Habilita este atributo
}

void display(double currentTime) {
    glUseProgram(renderingProgram);
    
    // clear the background to black, each time
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // move the triangle along x axis
    x += inc;
    // switch to moving the triangle to the left
    if (x > 1.0f) inc = -0.01f;
    // switch to moving the triangle to the right
    if (x < -1.0f) inc = 0.01f;
    
    // get ptr to "offset"
    GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");
    
    glProgramUniform1f(renderingProgram, offsetLoc, x);
    // Use este VAO e suas configura��es
    glBindVertexArray(m_VAO);
    //Draw Triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

    GLFWwindow* window = glfwCreateWindow(800, 800, "Lab06.0.4: Use Transformation Function with Shaders", NULL, NULL);
    glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
    glfwSwapInterval(1);
    
    init();
    
    while (!glfwWindowShouldClose(window)) {
        display(glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
