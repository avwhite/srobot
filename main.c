// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "linmath.h"
#include "controls.h"
#include "shaders.h"
#include "box.h"

GLFWwindow* init_window() {
	GLFWwindow* window;

	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return NULL;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	 
	window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental=1; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return NULL;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	return window;
}
	

int main(){
	printf("list created");

	GLFWwindow* window;
	Box box;
	EntityList entities;
	mat4x4 proj, view, model, temp, MVP;
	vec3 lightPos = {0,10,0};
	int i;

	window = init_window();
	if(window == NULL) { return -1; }

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	entities = createEntityList(5);

	box = createBox(1,1,1);
	insertEntity(&entities, &box);
	box = createBox(0.5, 2, 0.5);
	box.z = -1;
	insertEntity(&entities, &box);

	GLuint programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );
	GLuint mvpID = glGetUniformLocation(programID, "MVP");
	GLuint mID = glGetUniformLocation(programID, "M");
	GLuint vID = glGetUniformLocation(programID, "V");
	GLuint lightPosID = glGetUniformLocation(programID, "lightPosition_worldspace");

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	initControls();
	updateControlMatrices(window, view, proj);

	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		// For each model you render, since the MVP will be different (at least the M part)
		
		updateControlMatrices(window, view, proj);
		mat4x4_mul(temp, proj, view);

		glUniformMatrix4fv(vID, 1, GL_FALSE, view);
		glUniform3f(lightPosID, lightPos[0], lightPos[1], lightPos[2]);

		for (i = 0; i < entities.length; ++i) {
			Box *b = getEntity(&entities, i);

			getBoxModelMatrix(b, model);
			mat4x4_mul(MVP, temp, model);
			glUniformMatrix4fv(mvpID, 1, GL_FALSE, MVP);
			glUniformMatrix4fv(mID, 1, GL_FALSE, model);

			renderBox(b);
		}
		 
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	 
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	deleteEntityList(&entities);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}
