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
const float toRadians = 3.14159265f / 100.0f;

GLuint VAO, VBO, IBO, shader, uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

//Vertex Shader (taken each vertice, allow to modify values and pass to fragement shader)
//set the version of GLSL (openGL shader language)
static const char* vShader = "									\n\
#version 330													\n\
																\n\
layout(location = 0) in vec3 pos;								\n\
																\n\
out vec4 vColor;												\n\
																\n\
uniform mat4 model;												\n\
uniform mat4 projection;										\n\
																\n\
void main(){													\n\
	gl_Position = projection * model * vec4(pos, 1.0);						\n\
	vColor = 	vec4(clamp(pos, 0.0f, 1.0f), 1.0f);					\n\
}";

//Fragment Shader (handles each fragment/chunk/pixel on screen and how each one works with these vertices values that are part of vertex values)
static const char* fShader = "									\n\
#version 330													\n\
																\n\
in vec4 vColor;													\n\
																\n\
out vec4 colour;												\n\
																\n\
void main(){													\n\
	colour = vColor;											\n\
}";

void CreateTriangle() {

	unsigned int indices[] = {
		0, 3, 1, //change order which should be drawn first
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	//location of vertices
	GLfloat vertices[] = {
		-1.f, -1.f, 0.f, //front bottem left
		0.0f, -1.0f, 1.0f, //back middle bottom
		1.f, -1.f, 0.f, //front bottom right
		0.f, 1.f, 0.f,	//front middle top
	};

	//creating VAO (Vertex array object)
	glGenVertexArrays(1, &VAO); //(amount of array to create, store ID of array)
	glBindVertexArray(VAO); 

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	//creating VBO(Vertex buffer object)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//conect the vertices to the buffer for the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW); //Static Draw does not change the values when added (dynamic does)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
	uniformProjection = glGetUniformLocation(shader, "projection");
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
	
	GLFWwindow	*mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test window", NULL, NULL);
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

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		printf("Error: %s", glewGetErrorString(error));	glfwDestroyWindow(mainWindow);
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //determine object are deeper into image (draw on top of the others)

	//Setup Viewport size
	glViewport(0,0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

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

		curAngle += 0.001f;
		if (curAngle >= 360) {
			curAngle -= 360;
		}

		if (sizeDirection) {
			curSize += 0.0001f;
		}
		else {
			curSize -= 0.0001f;
		}
		if (curSize >= maxSize || curSize <= minSize) {
			sizeDirection = !sizeDirection;
		}

		// clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader); //grab ID after create, compile shader and everything (using this shader)

		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		//glUniform1f(uniformModel, triOffset);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection ));

		glBindVertexArray(VAO); // (using this VAO)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			//glDrawArrays(GL_TRIANGLES, 0, 3);//(Modes, start of aray, amount of points)
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0); //unbind

		glUseProgram(0); //unassign the shader

		glfwSwapBuffers(mainWindow);
	}
		
	return 0;
}