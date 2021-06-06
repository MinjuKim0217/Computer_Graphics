
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>
#include<algorithm>

using namespace std;

GLuint programID;
GLint colorLoc;
int r, g, b;
int click = 0;
float vertices[12];
int keychk = 0;

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
	//create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	//Read the vertex shader code from the file
	string VertexShaderCode;
	ifstream VertexShaderStream(vertex_file_path, ios::in);
	if (VertexShaderStream.is_open())
	{
		string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	//Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	//Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	vector<char> VertexShaderErrorMessage(max(InfoLogLength, int(1)));
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	//Read the fragment shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path, ios::in);
	if (FragmentShaderStream.is_open())
	{
		string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	//Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	//Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	vector<char> FragmentShaderErrorMessage(max(InfoLogLength, int(1)));
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

	//Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


void renderScene(void)
{
	//Clear all pixels
	glClear(GL_COLOR_BUFFER_BIT);
	//Let's draw something here


	GLint attribA = glGetAttribLocation(programID, "vtxPosition");
	GLint colorA = glGetAttribLocation(programID, "a_color");
	GLuint buf[2];
	glGenBuffers(2, buf);
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // 만들어진 버퍼를 어레이 타입으로 쓰겠다
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// 위치 attribute
	glVertexAttribPointer(attribA, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(attribA);

	//컬러 attribute
	glVertexAttribPointer(colorA, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); 
	glEnableVertexAttribArray(colorA);


	if (click == 0) {
		vertices[3] = 1.0f;
		vertices[4] = 0.0f;
		vertices[5] = 0.0f;
		glDrawArrays(GL_LINES, 0, 2);
		glutPostRedisplay();
	}
	else if(click==1) {
		vertices[9] = 1.0f;
		vertices[10] = 0.0f;
		vertices[11] = 0.0f;
		glDrawArrays(GL_LINES, 0, 2);
		glutPostRedisplay();
	}
	
	if (keychk == 1)
	{
		vertices[3] = 0.0f;
		vertices[4] = 1.0f;
		vertices[5] = 0.0f;
		glDrawArrays(GL_LINES, 0, 2);
		glutPostRedisplay();
	}
	if (keychk == 2)
	{
		vertices[9] = 0.0f;
		vertices[10] = 0.0f;
		vertices[11] = 1.0f;
		glDrawArrays(GL_LINES, 0, 2);
		glutPostRedisplay();
	}
			

	
	
	//Double buffer
	glutSwapBuffers();
}


void init()
{
	//initilize the glew and check the errors.
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s' \n", glewGetErrorString(res));
	}

	//select the background color
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's':
		keychk = 1;
		glutPostRedisplay();
		break;
	case 'e':
		keychk = 2;

		glutPostRedisplay();
		break;

	}
}
void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		click++;
		click = click % 2;
		glUniform4f(colorLoc, 1.0, 0.3, 0.0, 0.0);
		glDrawArrays(GL_LINES, 0, 2);
		glutPostRedisplay();
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		click += 2;
		click = click % 2;
		glUniform4f(colorLoc, 0.0, 0.2, 1.0, 0.0);
		glDrawArrays(GL_LINES, 0, 2);
		glutPostRedisplay();
	}
}
int main(int argc, char** argv)
{
	//init GLUT and create Window
	//initialize the GLUT
	glutInit(&argc, argv);
	//GLUT_DOUBLE enables double buffering (drawing to a background buffer while the other buffer is displayed)
	glutInitDisplayMode(/*GLUT_3_2_CORE_PROFILE*|*/ GLUT_DOUBLE | GLUT_RGBA);
	//These two functions are used to define the position and size of the window. 
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(480, 480);
	//This is used to define the name of the window.
	glutCreateWindow("Simple OpenGL Window");

	//call initization function
	init();

	//1.
	//Generate VAO
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//3. 
	programID = LoadShaders("VertexShader.txt", "FragmentShader.txt");
	glUseProgram(programID);


	float vertices[12] = { -0.9f, -0.9f, 0.0f,1.0f, 0.0f, 0.0f
,0.85f, -0.9f, 0.0f,0.0f, 1.0f, 0.0f };

	colorLoc = glGetUniformLocation(programID, "color");
	glUniform4f(colorLoc, r, g, b, 1);

	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	//resiter callbacks
	
	glutDisplayFunc(renderScene);

	//enter GLUT event processing cycle
	glutMainLoop();

	glDeleteVertexArrays(1, &VertexArrayID);

	return 1;
}

