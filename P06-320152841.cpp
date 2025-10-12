/*
Práctica 6: Texturizado
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture dado8Texture;
Texture logofiTexture;
Texture rinTexture;
Texture cauchoTexture;
Texture ojosTexture;
Texture fiTexture;

Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model Dadodae;

//Modelos del coche
Model Chasis_M;
Model Cofre_M;
Model Rueda_FD;
Model Rueda_FI;
Model Rueda_TD;
Model Rueda_TI;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}



void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);



	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.0f,  0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.25f,	0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.25f,	0.66f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.0f,	0.66f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.75f,  0.34f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.5f,	0.34f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.5f,	0.66f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.75f,	0.66f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.75f,  0.34f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,	0.34f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	0.66f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.75f,	0.66f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.5f,   0.34f,		0.0f,	-1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.75f,	0.34f,		0.0f,	-1.0f,	0.0f,
		0.5f,  -0.5f,  -0.5f,	0.75f,	0.0f,		0.0f,	-1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.5f,	0.0f,		0.0f,	-1.0f,	0.0f,

		//UP
		 //x		y		z		S		T

		-0.5f, 0.5f,  0.5f,	0.5f,  0.66f,		0.0f,	-1.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	0.75f,	0.66f,		0.0f,	-1.0f,	0.0f,
		0.5f, 0.5f,  -0.5f,	0.75f,	1.0f,		0.0f,	-1.0f,	0.0f,
		-0.5f, 0.5f,  -0.5f,	0.5f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

//Se crea el dado de 8 caras

void CrearDado8()
{
	unsigned int dado_indices[] = {
		// front
		0, 1, 2,
		3, 4, 5,

		// back
		6, 7, 8,
		9, 10, 11,

		// left
		12, 13, 14,
		15, 16, 17,
		// bottom
		18, 19, 20,
		21, 22, 23,
	};
	//Remplazar caras por el texturizado, agregar normales
	GLfloat dado_vertices[] = {
		// Cara 1 – superior frontal derecha (1)
		////x		y		z	S		T		NX		NY		NZ
		0.0f,  1.0f,  0.0f, 0.32f, 0.21f,		0.58f,  0.58f,  0.58f, //0
		0.0f,  0.0f,  1.0f,	0.015f, 0.39f,		0.58f,  0.58f,  0.58f,//1
		1.0f,  0.0f,  0.0f,	0.02f, 0.025f,		0.58f,  0.58f,  0.58f,//2

		// Cara 2 – superior derecha trasera (4)
		//x		y		z	S		T		
		0.0f,  1.0f,  0.0f,	0.34f, 0.21f,		0.58f,  0.58f, -0.58f,//3
		1.0f,  0.0f,  0.0f,	0.66f, 0.02f,		0.58f,  0.58f, -0.58f, //4
		0.0f,  0.0f, -1.0f,	0.66f, 0.39f,		0.58f,  0.58f, -0.58f, //5

		// Cara 3 – superior trasera izquierda (3)
		//x		y		z	  S		T
		0.0f,  1.0f,  0.0f,	 0.335f, 0.21f,	   -0.58f,  0.58f, -0.58f, //6
		0.0f,  0.0f, -1.0f,	 0.65f, 0.4f,	   -0.58f,  0.58f, -0.58f, //7
		-1.0f,  0.0f,  0.0f, 0.34f, 0.59f,	   -0.58f,  0.58f, -0.58f, //8

		// Cara 4 – superior izquierda frontal (2)
		//x		y		z	      S		T
		0.0f,  1.0f,   0.0f,	0.335f, 0.21f,	   -0.58f,  0.58f,  0.58f, //9
		-1.0f,  0.0f,  0.0f,	0.33f, 0.59f,	   -0.58f,  0.58f,  0.58f, //10
		0.0f,  0.0f,   1.0f,	0.04f, 0.39f,	   -0.58f,  0.58f,  0.58f, //11

		// Cara 5 – inferior frontal derecha (8)
		//x		y		z	S		T
		0.0f, -1.0f,  0.0f,	0.665f, 0.79f,		0.58f, -0.58f,  0.58f, //12
		1.0f,  0.0f,  0.0f,	0.985f, 0.6f,		0.58f, -0.58f,  0.58f, //13
		0.0f,  0.0f,  1.0f,	0.985f, 0.985f,		0.58f, -0.58f,  0.58f, //14

		// Cara 6 – inferior derecha trasera (7)
		//x		y		z	  S		 T
		0.0f, -1.0f,  0.0f,	0.665f, 0.79f,		0.58f, -0.58f, -0.58f, //15
		0.0f,  0.0f, -1.0f,	0.68f, 0.425f,		0.58f, -0.58f, -0.58f, //16
		1.0f,  0.0f,  0.0f,	0.985f, 0.6f,		0.58f, -0.58f, -0.58f, //17

		// Cara 7 – inferior trasera izquierda (6)
		//x		y		z	   S	T
		0.0f, -1.0f,  0.0f,	 0.66f, 0.79f,	   -0.58f, -0.58f, -0.58f, //18
		-1.0f,  0.0f,  0.0f, 0.36f, 0.6f,	   -0.58f, -0.58f, -0.58f, //19
		0.0f,  0.0f, -1.0f,	 0.66f, 0.43f,	   -0.58f, -0.58f, -0.58f, //20

		// Cara 8 – inferior izquierda frontal (5)
		//x		y		z	   S	T
		0.0f, -1.0f,  0.0f,	 0.64f, 0.79f,	   -0.58f, -0.58f,  0.58f, //21
		0.0f,  0.0f,  1.0f,	 0.34f, 0.96f,	   -0.58f, -0.58f,  0.58f, //22
		-1.0f,  0.0f,  0.0f, 0.34f, 0.61f,	   -0.58f, -0.58f,  0.58f, //23
	};

	Mesh* dado8 = new Mesh();
	dado8->CreateMesh(dado_vertices, dado_indices, 192, 36);
	meshList.push_back(dado8);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CreateShaders();
	CrearDado8();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado_nuevo.png");
	dadoTexture.LoadTextureA();
	logofiTexture = Texture("Textures/escudo_fi_color.tga");
	logofiTexture.LoadTextureA();
	//Se establece la textura para el dado de 8 caras
	dado8Texture = Texture("Textures/dado8.png");
	dado8Texture.LoadTextureA();

	rinTexture = Texture("Textures/rin_textura.png");
	rinTexture.LoadTextureA();
	cauchoTexture = Texture("Textures/caucho.png");
	cauchoTexture.LoadTextureA();
	ojosTexture = Texture("Textures/ojos.png");
	ojosTexture.LoadTextureA();





	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Dadodae = Model();
	Dadodae.LoadModel("Models/dadodae.dae");

	//Coche
	Chasis_M = Model();
	Chasis_M.LoadModel("Models/Chasis.dae");
	Cofre_M = Model();
	Cofre_M.LoadModel("Models/Cofre.dae");
	Rueda_FD = Model();
	Rueda_FD.LoadModel("Models/Rueda_FD.dae");
	Rueda_FI = Model();
	Rueda_FI.LoadModel("Models/Rueda_FI.dae");
	Rueda_TD = Model();
	Rueda_TD.LoadModel("Models/Rueda_TD.dae");
	Rueda_TI = Model();
	Rueda_TI.LoadModel("Models/Rueda_TI.dae");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelcar(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::mat4 modelaux3(1.0);
	glm::mat4 modelaux4(1.0);

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();


		//Dado de Opengl
		//Ejercicio 1: Texturizar su cubo con la imagen dado_animales ya optimizada por ustedes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Ejercicio 2:Importar el cubo texturizado en el programa de modelado con 
		//la imagen dado_animales ya optimizada por ustedes

		//Dado importado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, 3.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dadodae.RenderModel();




		/*Reporte de práctica :
		Ejercicio 1: Crear un dado de 8 caras y texturizarlo por medio de código*/

		//Mandar a llamar a la función del dado de 8 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, 10.0f, 25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado8Texture.UseTexture();
		meshList[5]->RenderMesh();


		/*Ejercicio 2: Importar el modelo de su coche con sus 4 llantas acomodadas
		y tener texturizadas las 4 llantas (diferenciar caucho y rin)  y
		texturizar el logo de la Facultad de ingeniería en el cofre de su propio modelo de coche*/


		//Se implementa el Coche con todas sus partes y siguiendo la jerarquía de modelos

		//Chasis (Principal)
		
		modelcar = glm::mat4(1.0);
		modelcar = glm::translate(modelcar, glm::vec3(0.0f, 14.0f, 0.0f));
		modelcar = glm::rotate(modelcar, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelcar = glm::scale(modelcar, glm::vec3(0.2f, 0.2f, 0.2f));
		modelcar = glm::translate(modelcar, glm::vec3(0.0f, 0.0f, mainWindow.getarticulacion6())); //hace que se mueva todo el coche
		modelcar = glm::translate(modelcar, glm::vec3(0.0f, 0.0f, mainWindow.getarticulacion7())); //hace que se mueva todo el coche
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelcar));
		Chasis_M.RenderModel();

		//Para manejar la jeraquía de los modelos
		modelaux = modelcar;
		modelaux2 = modelcar;
		modelaux3 = modelcar;
		modelaux4 = modelcar;

		//Se crea el Cofre
		/*modelaux = model;
		modelcar = glm::translate(modelcar, glm::vec3(2.2f, 3.3f, -18.0f));
		modelcar = glm::rotate(modelcar, glm::radians(mainWindow.getarticulacion1()), glm::vec3(-1.0f, 0.0f, 0.0f));
		modelcar = glm::rotate(modelcar, glm::radians(mainWindow.getarticulacion8()), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelcar));
		Cofre_M.RenderModel();*/

		//Solo para el cahsis (descomentar si se quiere ver el cofre)
		modelaux = model;
		modelcar = glm::translate(modelcar, glm::vec3(5.0f, 80.0f, 15.0f));
		modelcar = glm::rotate(modelcar, glm::radians(mainWindow.getarticulacion1()), glm::vec3(-1.0f, 0.0f, 0.0f));
		modelcar = glm::rotate(modelcar, glm::radians(mainWindow.getarticulacion8()), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelcar));
		Cofre_M.RenderModel();

		//Se crean las ruedas

		//Rueda_FD (Frontal Derecha)

		model = glm::mat4(1.0); //Se vuelve al chasis 

		modelaux = modelcar;
		modelcar = glm::translate(modelcar, glm::vec3(17.0f, -12.5f, -10.0f));
		modelcar = glm::rotate(modelcar, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelcar = glm::scale(modelcar, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelcar));
		Rueda_FD.RenderModel();

		//Rueda_DI (Frontal Izquierda)
		modelcar = modelaux2;
		modelcar = glm::translate(modelcar, glm::vec3(-17.0f, -10.0f, -28.0f));
		modelcar = glm::rotate(modelcar, glm::radians(mainWindow.getarticulacion3()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelcar = glm::scale(modelcar, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelcar));
		Rueda_FI.RenderModel();

		//Rueda_TD (Trasera Derecha)
		modelcar = modelaux3;
		modelcar = glm::translate(modelcar, glm::vec3(20.0f, -10.0f, 35.0f));
		modelcar = glm::rotate(modelcar, glm::radians(mainWindow.getarticulacion4()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelcar = glm::scale(modelcar, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelcar));
		Rueda_TD.RenderModel();

		//Rueda_TI (Rueda Izquierda)
		modelcar = modelaux4;
		modelcar = glm::translate(modelcar, glm::vec3(-16.5f, -10.0f, 35.0f));
		modelcar = glm::rotate(modelcar, glm::radians(mainWindow.getarticulacion5()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelcar = glm::scale(modelcar, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelcar));
		Rueda_TI.RenderModel();






















		
		////Instancia del coche 
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), -0.5f, -3.0f));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Kitt_M.RenderModel();

		////Llanta delantera izquierda
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////Llanta trasera izquierda
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////Llanta delantera derecha
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////Llanta trasera derecha
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/