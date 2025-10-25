#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	//Para la reversa
	GLfloat getmuevexR() { return muevexR; }
	//Para helicoptero
	GLfloat getmueveXHe() { return mueveXHe; }

	//-----------Práctica 8-------------------
	//para saber el estado de la luz
	GLboolean getprendida() { return luzprendida; }
	//UFO
	GLboolean getprendidaUFO() { return UFOPrendido; }
	//para el cofre del coche
	GLfloat getmuevey() { return muevey; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getarticulacion1() { return articulacion1; }
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat articulacion1, articulacion8;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	//para Helicoptero
	GLfloat mueveXHe;
	//Coche faro trasero
	GLfloat muevexR;


	//Practica 8
	//Se declara la variable
	GLboolean luzprendida;
	GLboolean UFOPrendido;
	//Cofre del coche
	GLfloat muevey;

	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

