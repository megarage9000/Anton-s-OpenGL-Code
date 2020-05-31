#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// Setting up Visual Studio for OpenGL:
// https://www.wikihow.com/Set-Up-OpenGL-GLFW-GLEW-GLM-on-a-Project-with-Visual-Studio



int main() {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return -1;
	}

	// ---- Creating GLFW window ----
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// ---- Creating GLEW extension handler ----
	glewExperimental = GL_TRUE;
	glewInit();

	// ---- Version Information ----
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported: %s\n", version);

	// Tells GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth testing
	glDepthFunc(GL_LESS); // depth-interprets a smaller value as "closer"


	// ---- Making a Triangle ----

	GLfloat trianglePoints[] = {
		0.5f, 0.5f, 0.0f, // Top point
		0.5f, -0.5f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f // Bottom Left
	};

	GLfloat trianglePoints2[] = {
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f, 
		-0.5f, -0.5f, 0.0f
	};

	// Triangle 1

	// Generating an empty buffer
	GLuint vbo = 0; // GLuint is a consistent typedef classification of openGL's unsigned int, ideal for OpenGL stuff
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePoints), trianglePoints, GL_STATIC_DRAW);
	// ^^^ This Line ^^^: Tells OpenGL size of buffer and element location

	// Generating a Vertrex Attribute Object
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Triangle 2
	GLuint vbo2 = 1;
	glGenBuffers(1, &vbo2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePoints2), trianglePoints2, GL_STATIC_DRAW);

	GLuint vao2 = 0;
	glGenVertexArrays(1, &vao2);
	glBindVertexArray(vao2);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	// Writing our Vertex Shader:
	const char* vertexShader =
		"#version 410\n"
		"in vec3 vp;"
		"void main(){"
		" gl_Position = vec4(vp, 1.0);"
		"}";

	// Writing our Fragment Shader:
	const char* fragmentShader =
		"#version 410\n"
		"out vec4 frag_colour;"
		"void main(){"
		" frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
		"}";

	// Loading Shaders
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShader, NULL);
	glCompileShader(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShader, NULL);
	glCompileShader(fs);

	// Creating a Shader Program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);


	// Drawing the Triangle 
	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(vao2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;

}