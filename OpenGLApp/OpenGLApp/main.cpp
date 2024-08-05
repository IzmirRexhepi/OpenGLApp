#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <ext/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"

const float toRadians = 3.14159265f / 100.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Light mainLight;

//Vertex Shader (taken each vertice, allow to modify values and pass to fragement shader)
//set the version of GLSL (openGL shader language)
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader (handles each fragment/chunk/pixel on screen and how each one works with these vertices values that are part of vertex values)
static const char* fShader = "Shaders/shader.frag";

void CreateObjects() {

	unsigned int indices[] = {
		0, 3, 1, //change order which should be drawn first
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	//location of vertices
	GLfloat vertices[] = {
		//	  x	     y    u		 u		v
		   -1.0f, -1.0f, 0.0f,  0.0f,  0.0f,			//front bottem left  Text:(bottem left)
			0.0f, -1.0f, 1.0f,	0.5f,  0.0f,			//back middle bottom Text:(bottom middle)
			1.0f, -1.0f, 0.0f,	1.0f,  0.0f,			//front bottom right Text:(bottem right)
			0.0f,  1.0f, 0.0f,	0.5f,  1.0f				//front middle top   Text:(top middle)
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 20, 12); //20 = amount of vertices passing (5*4= 20), 12 = number of indices
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj2);
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() {

	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 2.0f, 0.05f);

	brickTexture = Texture((char*)("Textures/brick.png")); //fix it idk how
	brickTexture.LoadTexture();
	dirtTexture = Texture((char*)("Textures/dirt.png"));
	dirtTexture.LoadTexture();

	mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f);
	//mainLight = Light(1.0f, 1.0f, 1.0f, 1.0f);
	//mainLight = Light(1.0f, 0.0f, 0.0f, 1.0f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformAmbientIntensity =  0, uniformAbientColour = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Loop until window closed
	while (!mainWindow.getShouldClose()) {
		GLfloat now = (GLfloat)glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get and Gandle user input event
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformAbientColour = shaderList[0].GetAmbientColourLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();

		mainLight.UseLight(uniformAmbientIntensity, uniformAbientColour);

		glm::mat4 model(1.0f);

		//render mesh
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		brickTexture.UseTexture();
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		meshList[1]->RenderMesh();

		glUseProgram(0); //unassign the shader

		mainWindow.SwapBuffers();
	}

	return 0;
}