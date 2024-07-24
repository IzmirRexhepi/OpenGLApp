#include <stdio.h>
#include <string.h>

#include <glew.h>
#include <glfw3.h>

//Windows dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

//Vertex Shader (taken each vertice, allow to modify values and pass to fragement shader)
//set the version of GLSL (openGL shader language)
static const char* vShader = "									\n\
#version 330													\n\
																\n\
layout(location = 0) in vec3 pos;								\n\
																\n\
void main(){													\n\
	gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);	\n\
}";

//Fragment Shader (handles each fragment/chunk/pixel on screen and how each one works with these vertices values that are part of vertex values)
static const char* fShader = "									\n\
#version 330													\n\
																\n\
out vec4 colour;												\n\
																\n\
void main(){													\n\
	colour = vec4(0.0, 0.0, 0.0, 1.0);							\n\
}";

void CreateTriangle() {
	//location of vertices
	GLfloat vertices[] = {
		-1.f, -1.f, 0.f, //bottem left
		1.f, -1.f, 0.f, //bottom right
		0.f, 1.f, 0.f,	//middle top
	};

	//creating VAO (Vertex array object)
	glGenVertexArrays(1, &VAO); //(amount of array to create, store ID of array)
	glBindVertexArray(VAO); 
	
	//creating VBO(Vertex buffer object)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//conect the vertices to the buffer for the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW); //Static Draw does not change the values when added (dynamic does)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void CompileShaders() {
	shader = glCreateProgram();
	if (!shader) {
		printf("Error creating shader program\n");
		return;
	}

}

int main() {

	//Initialise GLFW
	if (!glfwInit()) {
		printf("GLFW initalisation failed!");
		glfwTerminate();
		return 1;
	}

	//Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Core profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibiltty
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test window", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//Setup Viewport size
	glViewport(0,0, bufferWidth, bufferHeight);

	//Loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) {
		// Get and Gandle user input event
		glfwPollEvents();

		// clear window
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);
	}
		
	return 0;
}