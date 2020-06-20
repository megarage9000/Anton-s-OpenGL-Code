#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <time.h>

// This header allows variable amount of arguments
#include <stdarg.h>
#define GL_LOG_FILE "gl.log"

// TODO:
// - Figure out how to use fopen_s and other s variants for c functions used here ( We don't want to be using Depracted functions)
// - Do a bit of C programming! It will help us program more efficiently!
//		- Do the tutorials here: https://www.programiz.com/c-programming

// Setting up Visual Studio for OpenGL:
// https://www.wikihow.com/Set-Up-OpenGL-GLFW-GLEW-GLM-on-a-Project-with-Visual-Studio

bool restart_gl_log();
bool gl_log(const char* message, ...);
bool gl_log_err(const char* message, ...);
void glfw_error_callback(int error, const char* description);

int main() {

	if (!restart_gl_log()) {
		// Quit?
	}

	// Start context
	gl_log("starting GLFW\n%s\n", glfwGetVersionString());

	// Register our callback error function
	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return -1;
	}

	// ---- Setting window creation to be OpenGL 3.2 forward compatible core profile ----
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
		0.0f, 0.5f, 0.0f, // Top point
		0.5f, -0.5f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f // Bottom Left
	};

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
		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;

}

// Opens log file and prints time and date on top
bool restart_gl_log() {
	// See the following page for fopen_s:
	// https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm
	FILE* file = fopen(GL_LOG_FILE, "w");
	if (!file) {
		fprintf(
			stderr,
			"ERROR: could not open GL_LOG_FILE log file %s for writing\n",
			GL_LOG_FILE
		);
		return false;
	}

	time_t now = time(NULL);
	char* date = ctime(&now);
	fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
	fclose(file);
	return true;
}

// See this page on what the ... means:
// https://stackoverflow.com/questions/2735587/in-a-c-function-declaration-what-does-as-the-last-parameter-do
bool gl_log(const char* message, ...) {

	va_list argptr;
	FILE* file = fopen(GL_LOG_FILE, "a");
	if (!file) {
		fprintf(
			stderr,
			"ERROR: could not open GL_LOG_FILE %s file for appending\n",
			GL_LOG_FILE
		);
		return false;
	}
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}


bool gl_log_err(const char* message, ...) {
	
	va_list argptr;
	FILE* file = fopen(GL_LOG_FILE, "a");
	if (!file) {
		fprintf(
			stderr,
			"ERROR: could not open GL_LOG_FILE %s file for appending\n",
			GL_LOG_FILE
		);
		return false;
	}
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);

	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

// Logs error messages in which occured in GLFW initialisation
void glfw_error_callback(int error, const char* description) {
	gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}