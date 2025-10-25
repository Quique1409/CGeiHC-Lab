/*
Práctica 8: Iluminación 2 
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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
//Agregada por mí
Texture logofiTexture;
Texture rinTexture;
Texture cauchoTexture;
Texture caucho2Texture;
Texture ojosTexture;
Texture fiTexture;
Texture lampTexture;
//Agregar la textura de dado 8
Texture dado8Texture;
Texture UFOTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

//Modelos del coche
Model Chasis_M;
Model Cofre_M;
Model Rueda_FD;
Model Rueda_FI;
Model Rueda_TD;
Model Rueda_TI;

//lampara
Model Lampara;

//UFO
Model UFO;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];



// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//Para la lampara
unsigned int lucesPuntuales;


//función de calculo de normales por promedio de vértices 
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
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
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
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, -1.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, -1.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, -1.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, -1.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

//Se agrega el cubo de 8 caras
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
		0.0f,  1.0f,  0.0f, 0.32f, 0.21f,		-0.58f,  -0.58f,  -0.58f, //0
		0.0f,  0.0f,  1.0f,	0.015f, 0.39f,		-0.58f,  -0.58f,  -0.58f,//1
		1.0f,  0.0f,  0.0f,	0.02f, 0.025f,		-0.58f,  -0.58f,  -0.58f,//2

		// Cara 2 – superior derecha trasera (4)
		//x		y		z	S		T		
		0.0f,  1.0f,  0.0f,	0.34f, 0.21f,		-0.58f,  -0.58f, 0.58f,//3
		1.0f,  0.0f,  0.0f,	0.66f, 0.02f,		-0.58f,  -0.58f, 0.58f, //4
		0.0f,  0.0f, -1.0f,	0.66f, 0.39f,		-0.58f,  -0.58f, 0.58f, //5

		// Cara 3 – superior trasera izquierda (3)
		//x		y		z	  S		T
		0.0f,  1.0f,  0.0f,	 0.335f, 0.21f,	   0.58f,  -0.58f, 0.58f, //6
		0.0f,  0.0f, -1.0f,	 0.65f, 0.4f,	   0.58f,  -0.58f, 0.58f, //7
		-1.0f,  0.0f,  0.0f, 0.34f, 0.59f,	   0.58f,  -0.58f, 0.58f, //8

		// Cara 4 – superior izquierda frontal (2)
		//x		y		z	      S		T
		0.0f,  1.0f,   0.0f,	0.335f, 0.21f,	   0.58f,  -0.58f,  -0.58f, //9
		-1.0f,  0.0f,  0.0f,	0.33f, 0.59f,	   0.58f,  -0.58f,  -0.58f, //10
		0.0f,  0.0f,   1.0f,	0.04f, 0.39f,	   0.58f,  -0.58f,  -0.58f, //11

		// Cara 5 – inferior frontal derecha (8)
		//x		y		z	S		T
		0.0f, -1.0f,  0.0f,	0.665f, 0.79f,		-0.58f, 0.58f,  -0.58f, //12
		1.0f,  0.0f,  0.0f,	0.985f, 0.6f,		-0.58f, 0.58f,  -0.58f, //13
		0.0f,  0.0f,  1.0f,	0.985f, 0.985f,		-0.58f, 0.58f,  -0.58f, //14

		// Cara 6 – inferior derecha trasera (7)
		//x		y		z	  S		 T
		0.0f, -1.0f,  0.0f,	0.665f, 0.79f,		-0.58f, 0.58f, 0.58f, //15
		0.0f,  0.0f, -1.0f,	0.68f, 0.425f,		-0.58f, 0.58f, 0.58f, //16
		1.0f,  0.0f,  0.0f,	0.985f, 0.6f,		-0.58f, 0.58f, 0.58f, //17

		// Cara 7 – inferior trasera izquierda (6)
		//x		y		z	   S	T
		0.0f, -1.0f,  0.0f,	 0.66f, 0.79f,	   0.58f, 0.58f, 0.58f, //18
		-1.0f,  0.0f,  0.0f, 0.36f, 0.6f,	   0.58f, 0.58f, 0.58f, //19
		0.0f,  0.0f, -1.0f,	 0.66f, 0.43f,	   0.58f, 0.58f, 0.58f, //20

		// Cara 8 – inferior izquierda frontal (5)
		//x		y		z	   S	T
		0.0f, -1.0f,  0.0f,	 0.64f, 0.79f,	   0.58f, 0.58f,  -0.58f, //21
		0.0f,  0.0f,  1.0f,	 0.34f, 0.96f,	   0.58f, 0.58f,  -0.58f, //22
		-1.0f,  0.0f,  0.0f, 0.34f, 0.61f,	   0.58f, 0.58f,  -0.58f, //23
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
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	//Las agrego
	logofiTexture = Texture("Textures/escudo_fi_color.tga");
	logofiTexture.LoadTextureA();
	rinTexture = Texture("Textures/rin_textura.png");
	rinTexture.LoadTextureA();
	cauchoTexture = Texture("Textures/caucho.png");
	cauchoTexture.LoadTextureA();
	caucho2Texture = Texture("Textures/caucho.png.001.png");
	caucho2Texture.LoadTextureA();
	ojosTexture = Texture("Textures/ojos.png");
	ojosTexture.LoadTextureA();

	//Se establece la textura para el dado de 8 caras
	dado8Texture = Texture("Textures/dado8.png");
	dado8Texture.LoadTextureA();

	//Lampara
	lampTexture = Texture("Textures/Lamp.png");
	lampTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

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

	//Lampara
	Lampara = Model();
	Lampara.LoadModel("Models/Lampara.dae");

	//UFO
	UFO = Model();
	UFO.LoadModel("Models/UFO.dae");
	
	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales

	unsigned int pointLightCount = 0;



	// ---Lampara------
	pointLights[0] = PointLight(1.0f, 0.75f, 0.0f,
		1.0f, 5.0f,
//		 K_c	k_l	  k_q
		10.0f, 9.0f, -30.0,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;


	// UFOOOOOO
	pointLights[1] = PointLight(1.0f, 0.0f, 1.0f,
		1.0f, 8.0f,
//		 K_c	k_l	  k_q
		50.0f, 30.0f, -50.0f,
		1.0f, 0.2f, 0.1f);
	pointLightCount++;

	PointLight activePointLights[MAX_POINT_LIGHTS];

	unsigned int spotLightCount = 0;

	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//SpotLight para helicoptero
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,
		1.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		-0.5f, -1.0f, 0.0f, //Para darle un efecto más real
		1.0f, 0.0f, 0.0f,
		35.f);
	spotLightCount++;

	//----------------Práctica 8---------------------
	//SpotLight para cofre de coche
	spotLights[2] = SpotLight(0.7f, 0.5f, 0.0f,
		1.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, //Para darle un efecto más real
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//Para los faros
	unsigned int activeSpotLightCount = spotLightCount;

	//Luz de faro azul
	SpotLight faroAzul = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.f);

	//Luz de faro Roja
	SpotLight faroRojo = SpotLight(1.0f, 0.0f, 0.0f,
		1.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.f);

	//UFOO
	PointLight lamparaON = pointLights[0];
	PointLight UFOON = pointLights[1];

	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		//Posición del coche
		glm::vec3 carPos = glm::vec3(mainWindow.getmuevex(), 3.0f, 0.0f);


		if (mainWindow.getsKeys()[GLFW_KEY_U])
		{
			//Luz del coche Azul (Adelante)
			spotLights[3] = faroAzul;
			glm::vec3 faroAPos = carPos + glm::vec3(-8.5f, 0.0f, 0.0f);
			spotLights[3].SetPos(faroAPos);
			activeSpotLightCount++;

		}
		else if (mainWindow.getsKeys()[GLFW_KEY_Y])
		{
			//Luz del coche Roja (Atras)
			spotLights[3] = faroRojo;
			glm::vec3 faroRPos = carPos + glm::vec3(13.0f, -1.5f, 0.0f);
			spotLights[3].SetPos(faroRPos);
			activeSpotLightCount++;
		}

		//Luz del cofre
		glm::vec3 cofrePos = glm::vec3(-3.5, mainWindow.getmuevey(), 0.0f);
		glm::vec3 luzCofrePos = carPos + cofrePos + glm::vec3(-1.0f, 2.0f, 0.0f);
		spotLights[2].SetPos(luzCofrePos);


		//Luz del Helicoptero
		glm::vec3 HeliPos = glm::vec3(mainWindow.getmueveXHe(), 10.0f, 20.0f);
		glm::vec3 LuzPos = HeliPos + glm::vec3(-2.5f, 0.0f, 0.0f);
		spotLights[1].SetPos(LuzPos);

		//lampara para práctica 8
		//apagar y prender lampara
		unsigned int activePointLightCount = 0;

		if (mainWindow.getprendida())
		{
			activePointLights[activePointLightCount] = pointLights[0];
			activePointLightCount++;

		}
		
		if (mainWindow.getprendidaUFO())
		{
			activePointLights[activePointLightCount] = pointLights[1];
			activePointLightCount++;
		}

		


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(activePointLights, activePointLightCount);
		shaderList[0].SetSpotLights(spotLights, activeSpotLightCount);





		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		//Helicoptero
		glm::mat4 modelHe(1.0);
		glm::mat4 auxHe(1.0);

		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);



		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		//COCHEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 3.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Chasis_M.RenderModel();

		model = glm::translate(model, glm::vec3(-12.0f, 15.0f, 90.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-7.5f, -2.3f, 4.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rueda_FI.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(9.0f, -2.3f, 4.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rueda_TI.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-7.5f, -2.3f, -5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rueda_FD.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(9.0f, -2.3f, -5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rueda_TD.RenderModel();
	

		// ---------------------------Ejercicios de Práctica 7 --------------------------------------------

		// Instancia del Helicoptero
		/*
		El helicoptero se mueve con
		H : Adelante
		J : Atras
		*/
		modelHe = glm::mat4(1.0);
		modelHe = glm::translate(modelHe, glm::vec3(0.0f + mainWindow.getmueveXHe(), 10.0f, 20.0));
		modelHe = glm::scale(modelHe, glm::vec3(1.0f, 1.0f, 1.0f));
		modelHe = glm::rotate(modelHe, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelHe = glm::rotate(modelHe, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelHe));
		Blackhawk_M.RenderModel();

		//------------------------------Práctica 8-----------------------------------------

		//Se agrega cubo de 8 caras
		//Mandar a llamar a la función del dado de 8 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, 10.0f, 25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado8Texture.UseTexture();
		meshList[4]->RenderMesh();


		//Se implementa Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -1.0f, -30.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampTexture.UseTexture();
		Lampara.RenderModel();

		//Se implementa UFO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 40.0f, -50.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		UFO.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
