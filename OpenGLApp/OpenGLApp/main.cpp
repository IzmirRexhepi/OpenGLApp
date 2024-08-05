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
#include "Material.h"

const float toRadians = 3.14159265f / 100.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

Material shinyMaterial;
Material dullMaterial;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Light mainLight;

//Vertex Shader (taken each vertice, allow to modify values and pass to fragement shader)
//set the version of GLSL (openGL shader language)
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader (handles each fragment/chunk/pixel on screen and how each one works with these vertices values that are part of vertex values)
static const char* fShader = "Shaders/shader.frag";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat *vertices, unsigned int verticeCount,
						unsigned int vLenght, unsigned int normalOffset) {
	for (size_t i = 0; i < indiceCount; i += 3) {
		unsigned int in0 = indices[i] * vLenght;
		unsigned int in1 = indices[i+1] * vLenght;
		unsigned int in2 = indices[i+2] * vLenght;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}
	for (size_t i = 0; i < verticeCount / vLenght; i++) {
		unsigned int nOffset = i * vLenght + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects() {

	unsigned int indices[] = {
		0, 3, 1, //change order which should be drawn first
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	//location of vertices
	GLfloat vertices[] = {
		//	  x	     y    u				 u		v		 nx   ny   nz
		   -1.0f, -1.0f, -0.6f,			0.0f,  0.0f,	0.0f,0.0f,0.0f,		//front bottem left  Text:(bottem left)
			0.0f, -1.0f, 1.0f,			0.5f,  0.0f,	0.0f,0.0f,0.0f,		//back middle bottom Text:(bottom middle)
			1.0f, -1.0f, -0.6f,			1.0f,  0.0f,	0.0f,0.0f,0.0f,		//front bottom right Text:(bottem right)
			0.0f,  1.0f, 0.0f,			0.5f,  1.0f,	0.0f,0.0f,0.0f		//front middle top   Text:(top middle)
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5); //32 = 8*4; 

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12); //32 = amount of vertices passing (8*4= 20), 12 = number of indices
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() {

	mainWindow = Window(1366, 768); //1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 2.0f, 0.05f);

	brickTexture = Texture((char*)("Textures/brick.png")); //fix it idk how
	brickTexture.LoadTexture();
	dirtTexture = Texture((char*)("Textures/dirt.png"));
	dirtTexture.LoadTexture();

	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(0.3f, 4);

	//mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f,
	//				2.0f, -1.0f, -2.0f, 1.0f); //direction of light, last one = intensity of diffuse light
	mainLight = Light(1.0f, 1.0f, 1.0f, 0.1f, 2.0f, -1.0f, -2.0f, 0.1f); // to demonstrate specular lighting
	//mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f, 2.0f, -1.0f, -2.0f, 1.0f); 
	//mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f);
	//mainLight = Light(1.0f, 1.0f, 1.0f, 1.0f);
	//mainLight = Light(1.0f, 0.0f, 0.0f, 1.0f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformAmbientIntensity = 0, uniformAbientColour = 0, uniformDirection = 0, uniformDiffuseIntensity = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
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
		uniformDirection = shaderList[0].GetDirectionLocation();
		uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		mainLight.UseLight(uniformAmbientIntensity, uniformAbientColour,
							uniformDiffuseIntensity, uniformDirection);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0f);

		//render mesh
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		glUseProgram(0); //unassign the shader

		mainWindow.SwapBuffers();
	}

	return 0;
}