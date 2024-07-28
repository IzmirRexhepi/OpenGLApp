#include <stdio.h>
#include <string.h>
#include <cmath>


#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <ext/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//glm::mat4 model;

//Windows dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader, uniformModel;

bool direction = true;
float triOffset = 0.f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

//Vertex Shader (taken each vertice, allow to modify values and pass to fragement shader)
//set the version of GLSL (openGL shader language)
static const char* vShader = "									\n\
#version 330													\n\
																\n\
layout(location = 0) in vec3 pos;								\n\
																\n\
uniform mat4 model;												\n\
																\n\
void main(){													\n\
	gl_Position = model * vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);	\n\
}";

//Fragment Shader (handles each fragment/chunk/pixel on screen and how each one works with these vertices values that are part of vertex values)
static const char* fShader = "									\n\
#version 330													\n\
																\n\
out vec4 colour;												\n\
																\n\
void main(){													\n\
	colour = vec4(1.0, 0.0, 0.0, 1.0);							\n\
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

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLenght[1];
	codeLenght[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLenght);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result); //get GL compile status after compiling of shader
	if (!result) {
		glGetProgramInfoLog(theShader, sizeof(eLog), NULL, eLog); //getting info log, passing into the elog char array
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders() {

	shader = glCreateProgram();

	if (!shader) {
		printf("Error creating shader program\n");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, & result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog); //getting info log, passing into the elog char array
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog); //getting info log, passing into the elog char array
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");
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

	CreateTriangle();
	CompileShaders();

	//Loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) {
		// Get and Gandle user input event
		glfwPollEvents();

		if (!direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}

		// clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader); //grab ID after create, compile shader and everything (using this shader)

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));

		//glUniform1f(uniformModel, triOffset);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO); // (using this VAO)
		glDrawArrays(GL_TRIANGLES, 0, 3);//(Modes, start of aray, amount of points)

		glBindVertexArray(0); //unbind

		glUseProgram(0); //unassign the shader

		glfwSwapBuffers(mainWindow);
	}
		
	return 0;
}